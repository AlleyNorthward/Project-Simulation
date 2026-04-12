import json
import random
import time
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, Optional

MIN_VALUE = 0
MAX_VALUE = 15
TOTAL_QUESTIONS = 100

HISTORY_FILE = Path("./build/conversion_history.json")
MISTAKE_FILE = Path("./build/conversion_mistakes.json")
SLOW_TIME_THRESHOLD = 3.0

def to_hex(n: int) -> str:
    return f"0x{n:x}"

def to_bin(n: int) -> str:
    return f"0b{n:04b}"

def to_dec(n: int) -> str:
    return str(n)

def base_to_key(base_text: str) -> Optional[str]:
    base_text = base_text.strip().lower()
    if base_text == "2":
        return "bin"
    if base_text == "10":
        return "dec"
    if base_text == "16":
        return "hex"
    return None

def key_to_label(key: str) -> str:
    return {"bin": "2", "dec": "10", "hex": "16"}[key]

def normalize_answer(text: str, base: str) -> str:
    s = text.strip().lower().replace(" ", "")

    if base == "hex":
        if s.startswith("0x"):
            s = s[2:]
        if not s:
            return "0x"
        try:
            return f"0x{int(s, 16):x}"
        except ValueError:
            return "0x" + s

    if base == "bin":
        if s.startswith("0b"):
            s = s[2:]
        if not s:
            return "0b"
        if all(c in "01" for c in s):
            try:
                return f"0b{int(s, 2):04b}"
            except ValueError:
                return "0b" + s.zfill(4)
        return "0b" + s

    return s

def get_mode():
    while True:
        mode = input("请输入模式, 例如 2 10 / 10 2 / 16 10 / 2 16 / all: ").strip().lower()

        if mode == "all":
            return "all", None, None

        parts = mode.replace("->", " ").split()
        if len(parts) == 2:
            src = base_to_key(parts[0])
            dst = base_to_key(parts[1])

            if src and dst and src != dst:
                return "fixed", src, dst

        print("输入无效, 请重新输入. 比如: 2 10、10 2、16 10、2 16、all")

@dataclass
class QuestionResult:
    index: int
    source_base: str
    target_base: str
    source_text: str
    expected_answer: str
    user_input: str
    normalized_user_input: str
    elapsed_seconds: float
    current_average_before_seconds: float

    @property
    def is_correct(self) -> bool:
        return self.normalized_user_input == normalize_answer(self.expected_answer, self.target_base)

    @property
    def is_timeout(self) -> bool:
        if self.current_average_before_seconds <= 0:
            return False

        return self.elapsed_seconds > self.current_average_before_seconds * 1.3

class JsonStore:
    def __init__(self, path: Path) -> None:
        self.path = path

    def load(self, default: Any) -> Any:
        if not self.path.exists():
            return default

        try:
            with self.path.open("r", encoding="utf-8") as f:
                return json.load(f)
        except Exception:
            return default

    def save(self, data: Any) -> None:
        with self.path.open("w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

class TrainingHistoryStore(JsonStore):
    def __init__(self, path: Path) -> None:
        super().__init__(path)
        self.data: Dict[str, Dict[str, Dict[str, Any]]] = self.load(default={})

    def add_run(self, mode_text: str, start_time: str, finish_time: str, total_time: float, correct_count: int) -> None:
        if mode_text not in self.data:
            self.data[mode_text] = {}

        mode_bucket = self.data[mode_text]
        next_index = self._next_index(mode_bucket)

        mode_bucket[str(next_index)] = {
            "current_time": start_time,
            "finish_time": finish_time,
            "total_time": round(total_time, 6),
            "correct_count": correct_count,
        }

        self.save(self.data)

    @staticmethod
    def _next_index(mode_bucket: Dict[str, Dict[str, Any]]) -> int:
        if not mode_bucket:
            return 1

        numeric_keys = []
        for k in mode_bucket.keys():
            try:
                numeric_keys.append(int(k))
            except ValueError:
                pass

        return max(numeric_keys, default=0) + 1

class MistakeStore(JsonStore):
    def __init__(self, path: Path) -> None:
        super().__init__(path)

        self.data = {
            "wrong_questions": [],
            "timeout_questions": [],
        }

        if "wrong_questions" not in self.data:
            self.data["wrong_questions"] = []
        if "timeout_questions" not in self.data:
            self.data["timeout_questions"] = []

    @staticmethod
    def _minimal_record(result: QuestionResult) -> Dict[str, Any]:
        return {
            "index": result.index,
            "source_text": result.source_text,
            "expected_answer": result.expected_answer,
            "user_input": result.user_input,
            "elapsed_seconds": round(result.elapsed_seconds, 6),
            "current_average_before_seconds": round(result.current_average_before_seconds, 6),
        }

    def add_wrong(self, result: QuestionResult) -> None:
        self.data["wrong_questions"].append(self._minimal_record(result))
        self.save(self.data)

    def add_timeout(self, result: QuestionResult) -> None:
        self.data["timeout_questions"].append(self._minimal_record(result))
        self.save(self.data)

class ConversionQuiz:
    def __init__(self) -> None:
        self.converters = {
            "hex": to_hex,
            "bin": to_bin,
            "dec": to_dec,
        }
        self.mode_type, self.fixed_src, self.fixed_dst = get_mode()
        self.mode_text = "all" if self.mode_type == "all" else f"{key_to_label(self.fixed_src)} {key_to_label(self.fixed_dst)}"

        self.history_store = TrainingHistoryStore(HISTORY_FILE)
        self.mistake_store = MistakeStore(MISTAKE_FILE)

        self.score = 0
        self.total_time = 0.0
        self.answered_count = 0

    def choose_pair(self) -> tuple[str, str]:
        if self.mode_type == "all":
            bases = ["hex", "bin", "dec"]
            src = random.choice(bases)
            dst = random.choice([b for b in bases if b != src])
            return src, dst
        return self.fixed_src, self.fixed_dst

    def generate_value(self, src: str, dst: str) -> int:
        if (src == "hex" and dst == "dec") or (src == "dec" and dst == "hex"):
            return random.randint(10, MAX_VALUE)
        return random.randint(MIN_VALUE, MAX_VALUE)

    def run(self) -> None:
        start_dt = datetime.now()
        start_time_text = start_dt.strftime("%Y-%m-%d %H:%M:%S")

        print("\n开始练习: ")
        print("十六进制用 0x 表示，二进制用 0b 表示，十进制直接输入数字.")
        print("二进制如果只输入 11, 会自动按 0b0011 处理.")
        print("-" * 40)

        for i in range(1, TOTAL_QUESTIONS + 1):
            src, dst = self.choose_pair()
            value = self.generate_value(src, dst)

            src_text = self.converters[src](value)
            expected = self.converters[dst](value)

            print(f"第 {i} 题: {src_text}  ->  {key_to_label(dst)}")

            current_average_before = self.total_time / self.answered_count if self.answered_count > 0 else 0.0

            start_time = time.perf_counter()
            user_input = input("答案: ")
            elapsed = time.perf_counter() - start_time

            self.total_time += elapsed
            self.answered_count += 1

            user_norm = normalize_answer(user_input, dst)
            expected_norm = normalize_answer(expected, dst)
            is_correct = user_norm == expected_norm

            result = QuestionResult(
                index=i,
                source_base=src,
                target_base=dst,
                source_text=src_text,
                expected_answer=expected,
                user_input=user_input,
                normalized_user_input=user_norm,
                elapsed_seconds=elapsed,
                current_average_before_seconds=current_average_before,
            )

            print(f"用时: {elapsed:.3f} 秒")

            if is_correct:
                print("对了.\n")
                self.score += 1
            else:
                print(f"错了, 正确答案是: {expected}")
                print("这个还需要练.\n")
                self.mistake_store.add_wrong(result)

            if result.is_timeout:
                self.mistake_store.add_timeout(result)
                print("这题已记录为超时题.\n")

        finish_dt = datetime.now()
        finish_time_text = finish_dt.strftime("%Y-%m-%d %H:%M:%S")

        self.history_store.add_run(
            mode_text=self.mode_text,
            start_time=start_time_text,
            finish_time=finish_time_text,
            total_time=self.total_time,
            correct_count=self.score,
        )

        print("-" * 40)
        print(f"结束, 正确 {self.score} / {TOTAL_QUESTIONS} 题")
        print(f"总用时: {self.total_time:.3f} 秒")
        print(f"平均每题: {self.total_time / TOTAL_QUESTIONS:.3f} 秒")
        print(f"结果已保存到: {HISTORY_FILE} 和 {MISTAKE_FILE}")

def main():
    quiz = ConversionQuiz()
    quiz.run()

if __name__ == "__main__":
    main()
