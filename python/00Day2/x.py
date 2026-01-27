import random

datas = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
         "-", "=", "!", "@", "#", "$", "%", "^", "&", "*",
         "(", ")", "_", "+", "|"]

length = 80

display = " ".join(random.choice(datas) for _ in range(length))
print("请输入下面这一行：")
print(display)

answer = display.replace(" ", "")

user_input = input("> ")

if len(user_input) != len(answer):
    print(f"❌ 长度不对：应输入 {len(answer)} 个字符，实际输入 {len(user_input)} 个")

for i, (c, u) in enumerate(zip(answer, user_input), start=1):
    if c != u:
        print(f"❌ 第 {i} 个错了：应为 '{c}'，你输入的是 '{u}'")

if len(user_input) < len(answer):
    for i in range(len(user_input) + 1, len(answer) + 1):
        print(f"❌ 第 {i} 个缺失，应为 '{answer[i-1]}'")

if user_input == answer:
    print("✅ 全对！这手速可以了 🔥")
