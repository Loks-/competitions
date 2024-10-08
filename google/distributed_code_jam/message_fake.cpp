#include "message.h"

#include "common/base.h"
#include "common/template.h"

#include <queue>

int NumberOfNodes() { return 1; }
int MyNodeId() { return 0; }

namespace {
static std::queue<long long> qp, qs, qr;
}

void PutChar(int target, char value) {
  MetaUse(target);
  assert(target == 0);
  qp.push(value);
}

void PutInt(int target, int value) {
  MetaUse(target);
  assert(target == 0);
  qp.push(value);
}

void PutLL(int target, long long value) {
  MetaUse(target);
  assert(target == 0);
  qp.push(value);
}

void Send(int target) {
  MetaUse(target);
  assert(target == 0);
  for (; !qp.empty(); qp.pop()) qs.push(qp.front());
}

int Receive(int source) {
  MetaUse(source);
  assert((source == 0) || (source == -1));
  assert(!qs.empty());
  for (; !qs.empty(); qs.pop()) qr.push(qs.front());
  return 0;
}

char GetChar(int source) {
  MetaUse(source);
  assert(source == 0);
  assert(!qr.empty());
  long long value = qr.front();
  qr.pop();
  return char(value);
}

int GetInt(int source) {
  MetaUse(source);
  assert(source == 0);
  assert(!qr.empty());
  long long value = qr.front();
  qr.pop();
  return int(value);
}

long long GetLL(int source) {
  MetaUse(source);
  assert(source == 0);
  assert(!qr.empty());
  long long value = qr.front();
  qr.pop();
  return value;
}
