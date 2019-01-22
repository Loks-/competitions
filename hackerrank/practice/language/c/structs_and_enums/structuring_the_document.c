// https://www.hackerrank.com/challenges/structuring-the-document

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

struct word {
  char* data;
};

struct sentence {
  struct word* data;
  int word_count;  // denotes number of words in a sentence
};

struct paragraph {
  struct sentence* data;
  int sentence_count;  // denotes number of sentences in a paragraph
};

struct document {
  struct paragraph* data;
  int paragraph_count;  // denotes number of paragraphs in a document
};

static int count(char* s, char c) {
  int r = 0;
  for (; *s;) {
    if (*s++ == c) ++r;
  }
  return r;
}

struct document get_document(char* text) {
  struct document d;
  int k = count(text, '\n') + 1;
  d.paragraph_count = k;
  d.data = (struct paragraph*)malloc(k * sizeof(struct paragraph));
  char* text_k = text;
  for (int ik = 0; ik < k; ++ik) {
    char* text_k_end = text_k;
    for (; *text_k_end && (*text_k_end != '\n');) ++text_k_end;
    *text_k_end = 0;
    int m = count(text_k, '.');
    d.data[ik].sentence_count = m;
    d.data[ik].data = (struct sentence*)malloc(m * sizeof(struct sentence));
    char* text_m = text_k;
    for (int im = 0; im < m; ++im) {
      char* text_m_end = text_m;
      for (; *text_m_end != '.';) ++text_m_end;
      *text_m_end = 0;
      int n = count(text_m, ' ') + 1;
      d.data[ik].data[im].word_count = n;
      d.data[ik].data[im].data = (struct word*)malloc(n * sizeof(struct word));
      char* text_n = text_m;
      for (int in = 0; in < n; ++in) {
        d.data[ik].data[im].data[in].data = text_n;
        for (; *text_n && (*text_n != ' ');) ++text_n;
        *text_n++ = 0;
      }
      text_m = text_m_end + 1;
    }
    text_k = text_k_end + 1;
  }
  return d;
}

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document doc,
                                                      int k, int m, int n) {
  return doc.data[n - 1].data[m - 1].data[k - 1];
}

struct sentence kth_sentence_in_mth_paragraph(struct document doc, int k,
                                              int m) {
  return doc.data[m - 1].data[k - 1];
}

struct paragraph kth_paragraph(struct document doc, int k) {
  return doc.data[k - 1];
}

static void print_word(struct word w) { printf("%s", w.data); }

static void print_sentence(struct sentence sen) {
  for (int i = 0; i < sen.word_count; i++) {
    print_word(sen.data[i]);
    if (i != sen.word_count - 1) {
      printf(" ");
    }
  }
}

static void print_paragraph(struct paragraph para) {
  for (int i = 0; i < para.sentence_count; i++) {
    print_sentence(para.data[i]);
    printf(".");
  }
}

static void print_document(struct document doc) {
  for (int i = 0; i < doc.paragraph_count; i++) {
    print_paragraph(doc.data[i]);
    if (i != doc.paragraph_count - 1) printf("\n");
  }
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

int main_structuring_the_document() {
  char* text = get_input_text();
  struct document Doc = get_document(text);

  int q;
  scanf("%d", &q);

  while (q--) {
    int type;
    scanf("%d", &type);

    if (type == 3) {
      int k, m, n;
      scanf("%d %d %d", &k, &m, &n);
      struct word w = kth_word_in_mth_sentence_of_nth_paragraph(Doc, k, m, n);
      print_word(w);
    }

    else if (type == 2) {
      int k, m;
      scanf("%d %d", &k, &m);
      struct sentence sen = kth_sentence_in_mth_paragraph(Doc, k, m);
      print_sentence(sen);
    }

    else {
      int k;
      scanf("%d", &k);
      struct paragraph para = kth_paragraph(Doc, k);
      print_paragraph(para);
    }
    printf("\n");
  }

  return 0;
}
