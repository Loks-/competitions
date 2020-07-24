#pragma once

class Node;

void EvaluationSkipDraw(bool skip_draw);
void ExpandAlias(Node* node);
void EvaluateLazyIsNil(Node* node);
void Evaluate(Node* node);
