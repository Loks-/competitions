// https://www.hackerrank.com/challenges/post-transition

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 6

struct package {
  char* id;
  int weight;
};

typedef struct package package;

struct post_office {
  int min_weight;
  int max_weight;
  package* packages;
  int packages_count;
};

typedef struct post_office post_office;

struct town {
  char* name;
  post_office* offices;
  int offices_count;
};

typedef struct town town;

static void print_all_packages(town t) {
  printf("%s:\n", t.name);
  for (int i = 0; i < t.offices_count; ++i) {
    printf("\t%d:\n", i);
    for (int j = 0; j < t.offices[i].packages_count; ++j)
      printf("\t\t%s\n", t.offices[i].packages[j].id);
  }
}

static void send_all_acceptable_packages(town* source, int source_office_index,
                                         town* target,
                                         int target_office_index) {
  post_office* s = &(source->offices[source_office_index]);
  post_office* t = &(target->offices[target_office_index]);
  int transfered = 0;
  for (int i = 0; i < s->packages_count; ++i) {
    if ((s->packages[i].weight >= t->min_weight) &&
        (s->packages[i].weight <= t->max_weight))
      ++transfered;
  }
  package* p =
      (package*)malloc((t->packages_count + transfered) * sizeof(package));
  for (unsigned i = 0; i < t->packages_count; ++i) p[i] = t->packages[i];
  package *ps = s->packages, *pt = p + t->packages_count;
  for (unsigned i = 0; i < s->packages_count; ++i) {
    if ((s->packages[i].weight >= t->min_weight) &&
        (s->packages[i].weight <= t->max_weight))
      *pt++ = s->packages[i];
    else
      *ps++ = s->packages[i];
  }
  s->packages_count -= transfered;
  t->packages_count += transfered;
  free(t->packages);
  t->packages = p;
}

static int packages_in_town(town t) {
  int s = 0;
  for (int i = 0; i < t.offices_count; ++i) s += t.offices[i].packages_count;
  return s;
}

static town town_with_most_packages(town* towns, int towns_count) {
  int max_packages = packages_in_town(towns[0]), best_index = 0;
  for (int i = 1; i < towns_count; ++i) {
    int p = packages_in_town(towns[i]);
    if (max_packages < p) {
      max_packages = p;
      best_index = i;
    }
  }
  return towns[best_index];
}

static town* find_town(town* towns, int towns_count, char* name) {
  for (int i = 0; i < towns_count; ++i) {
    if (!strcmp(towns[i].name, name)) return &towns[i];
  }
  return 0;
}

int main_post_transition() {
  int towns_count;
  scanf("%d", &towns_count);
  town* towns = malloc(sizeof(town) * towns_count);
  for (int i = 0; i < towns_count; i++) {
    towns[i].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
    scanf("%s", towns[i].name);
    scanf("%d", &towns[i].offices_count);
    towns[i].offices = malloc(sizeof(post_office) * towns[i].offices_count);
    for (int j = 0; j < towns[i].offices_count; j++) {
      scanf("%d%d%d", &towns[i].offices[j].packages_count,
            &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
      towns[i].offices[j].packages =
          malloc(sizeof(package) * towns[i].offices[j].packages_count);
      for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
        towns[i].offices[j].packages[k].id =
            malloc(sizeof(char) * MAX_STRING_LENGTH);
        scanf("%s", towns[i].offices[j].packages[k].id);
        scanf("%d", &towns[i].offices[j].packages[k].weight);
      }
    }
  }
  int queries;
  scanf("%d", &queries);
  char town_name[MAX_STRING_LENGTH];
  while (queries--) {
    int type;
    scanf("%d", &type);
    switch (type) {
      case 1:
        scanf("%s", town_name);
        town* t = find_town(towns, towns_count, town_name);
        print_all_packages(*t);
        break;
      case 2:
        scanf("%s", town_name);
        town* source = find_town(towns, towns_count, town_name);
        int source_index;
        scanf("%d", &source_index);
        scanf("%s", town_name);
        town* target = find_town(towns, towns_count, town_name);
        int target_index;
        scanf("%d", &target_index);
        send_all_acceptable_packages(source, source_index, target,
                                     target_index);
        break;
      case 3:
        printf("Town with the most number of packages is %s\n",
               town_with_most_packages(towns, towns_count).name);
        break;
    }
  }
  return 0;
}
