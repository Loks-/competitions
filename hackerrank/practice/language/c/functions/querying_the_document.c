// https://www.hackerrank.com/challenges/querying-the-document

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

static int count(char* s, char c) {
  int r = 0;
  for (; *s;) {
    if (*s++ == c) ++r;
  }
  return r;
}

static char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k,
                                                       int m, int n) {
  return document[n - 1][m - 1][k - 1];
}

static char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) {
  return document[m - 1][k - 1];
}

static char*** kth_paragraph(char**** document, int k) {
  return document[k - 1];
}

static char**** get_document(char* text) {
  int k = count(text, '\n') + 1;
  char**** p = (char****)malloc(k * sizeof(char***));
  char* text_k = text;
  for (int ik = 0; ik < k; ++ik) {
    char* text_k_end = text_k;
    for (; *text_k_end && (*text_k_end != '\n');) ++text_k_end;
    *text_k_end = 0;
    int m = count(text_k, '.');
    p[ik] = (char***)malloc(m * sizeof(char**));
    char* text_m = text_k;
    for (int im = 0; im < m; ++im) {
      char* text_m_end = text_m;
      for (; *text_m_end != '.';) ++text_m_end;
      *text_m_end = 0;
      int n = count(text_m, ' ') + 1;
      p[ik][im] = (char**)malloc(n * sizeof(char*));
      char* text_n = text_m;
      for (int in = 0; in < n; ++in) {
        p[ik][im][in] = text_n;
        for (; *text_n && (*text_n != ' ');) ++text_n;
        *text_n++ = 0;
      }
      text_m = text_m_end + 1;
    }
    text_k = text_k_end + 1;
  }
  return p;
}

static char* get_input_text() {
  int paragraph_count;
  scanf("%d", &paragraph_count);

  char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
  memset(doc, 0, sizeof(doc));
  getchar();
  for (int i = 0; i < paragraph_count; i++) {
    scanf("%[^\n]%*c", p[i]);
    strcat(doc, p[i]);
    if (i != paragraph_count - 1) strcat(doc, "\n");
  }

  char* returnDoc = (char*)malloc((strlen(doc) + 1) * (sizeof(char)));
  strcpy(returnDoc, doc);
  return returnDoc;
}

static void print_word(char* word) { printf("%s", word); }

static void print_sentence(char** sentence) {
  int word_count;
  scanf("%d", &word_count);
  for (int i = 0; i < word_count; i++) {
    printf("%s", sentence[i]);
    if (i != word_count - 1) printf(" ");
  }
}

static void print_paragraph(char*** paragraph) {
  int sentence_count;
  scanf("%d", &sentence_count);
  for (int i = 0; i < sentence_count; i++) {
    print_sentence(*(paragraph + i));
    printf(".");
  }
}

int main_querying_the_document() {
  char* text = get_input_text();
  char**** document = get_document(text);

  int q;
  scanf("%d", &q);

  while (q--) {
    int type;
    scanf("%d", &type);

    if (type == 3) {
      int k, m, n;
      scanf("%d %d %d", &k, &m, &n);
      char* word = kth_word_in_mth_sentence_of_nth_paragraph(document, k, m, n);
      print_word(word);
    }

    else if (type == 2) {
      int k, m;
      scanf("%d %d", &k, &m);
      char** sentence = kth_sentence_in_mth_paragraph(document, k, m);
      print_sentence(sentence);
    }

    else {
      int k;
      scanf("%d", &k);
      char*** paragraph = kth_paragraph(document, k);
      print_paragraph(paragraph);
    }
    printf("\n");
  }

  return 0;
}
