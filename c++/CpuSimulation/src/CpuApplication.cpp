#include "CpuApplication.h"
#include "AddStrategy.h"
#include "IncrementAllStrategy.h"
#include "MoveStrategy.h"
#include "RandomNoiseStrategy.h"
#include <windows.h>

namespace sdust {

void CpuApplication::initCpu() {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  cpu = new CPU;
  to = new TerminalObserver;
  cpu->attach(to);
}

void CpuApplication::configRegister() {
  cpu->desc().add("EAX", "EAX", {0, 32});
  cpu->desc().add("AX", "EAX", {0, 16});
  cpu->desc().add("AH", "EAX", {8, 8});
  cpu->desc().add("AL", "EAX", {0, 8});
  cpu->write("EAX", 0);

  cpu->desc().add("EBX", "EBX", {0, 32});
  cpu->desc().add("BX", "EBX", {0, 16});
  cpu->desc().add("BH", "EBX", {8, 8});
  cpu->desc().add("BL", "EBX", {0, 8});
  cpu->write("EBX", 0);

  cpu->desc().add("ECX", "ECX", {0, 32});
  cpu->desc().add("CX", "ECX", {0, 16});
  cpu->desc().add("CH", "ECX", {8, 8});
  cpu->desc().add("CL", "ECX", {0, 8});
  cpu->write("ECX", 0);

  cpu->desc().add("EDX", "EDX", {0, 32});
  cpu->desc().add("DX", "EDX", {0, 16});
  cpu->desc().add("DH", "EDX", {8, 8});
  cpu->desc().add("DL", "EDX", {0, 8});
  cpu->write("EDX", 0);

  cpu->desc().add("ESI", "ESI", {0, 32});
  cpu->desc().add("SI", "ESI", {0, 16});
  cpu->write("ESI", 0);

  cpu->desc().add("EDI", "EDI", {0, 32});
  cpu->desc().add("DI", "EDI", {0, 16});
  cpu->write("EDI", 0);

  cpu->desc().add("EBP", "EBP", {0, 32});
  cpu->desc().add("BP", "EBP", {0, 16});
  cpu->write("EBP", 0);

  cpu->desc().add("ESP", "ESP", {0, 32});
  cpu->desc().add("SP", "ESP", {0, 16});
  cpu->write("ESP", 0);

  cpu->desc().add("EIP", "EIP", {0, 32});
  cpu->write("EIP", 0);

  iStra = new IncrementAllStrategy(cpu);
  aStra = new AddStrategy(cpu);
  mStra = new MoveStrategy(cpu);
  rStra = new RandomNoiseStrategy(cpu);

  strategies.push_back(iStra);
  strategies.push_back(aStra);
  strategies.push_back(mStra);
  strategies.push_back(rStra);

  idx = 0;
  regStr = strategies[idx];
}

void CpuApplication::changeRegStr() {
  if (strategies.empty())
    return;

  idx = (idx + 1) % strategies.size();
  regStr = strategies[idx];
}

CpuApplication::~CpuApplication() {
  delete iStra;
  delete aStra;
  delete mStra;

  delete rStra;

  delete to;
  delete cpu;
}

} // namespace sdust
