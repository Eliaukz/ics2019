#include <stdlib.h>
#include <string.h>

#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP *new_wp(char* expression) {
  WP *res = free_;
  if (res == NULL) assert(0);
  free_ = free_->next;
  res->next = head;
  head = res;
  res->expression = (char *)malloc(strlen(expression) * sizeof(char));
  strcpy(res->expression, expression);
  bool success;
  res->value = expr(res->expression, &success);
  res->old_value = res->value;
  res->enable = true;
  return res;
}

void free_wp(WP *wp) {
  WP* ptr;

  if(head==wp){
    ptr = head;
    head = head->next;
    ptr->next = free_;
    free_ = ptr;
    wp->enable = false;
  } else {
    for (ptr = head; ptr != NULL && ptr->next != wp; ptr = ptr->next) {}

    if (ptr == NULL) {
      printf("not find such watchpoint\n");
    } else {
      ptr->next = wp->next;
      wp->next = free_;
      free_ = wp;
      wp->enable = false;
    }
  }
}

bool delete_wp(int no){
  WP *ptr;
  bool found = false;
  // printf("no%d\n", no);
  for (ptr = head; ptr != NULL; ptr = ptr->next) {
    if (ptr->NO == no) {
      free_wp(ptr);
      found = true;
      break;
    }
  }
  if(!found){
    printf("No such an activatd watchpoint\n");
  }
  return found;
}



bool change_wp() {
  bool has_changed = false;
  WP *ptr;
  for (ptr = head; ptr != NULL; ptr = ptr->next) {
    bool success;
    int val = expr(ptr->expression, &success);
    ptr->old_value = ptr->value;
    ptr->value = val;
    if(ptr->value!=ptr->old_value){
      has_changed = true;
    }
  }
  return has_changed;
}


void print_wp(){
  WP* ptr;
  printf("%-10s%-32s%-16s%-16s%-8s%-8s\n", "NO", "expression", "old_value",
         "value", "change", "enable");
  for (ptr = head; ptr != NULL; ptr = ptr->next)  {
    char changed = (ptr->old_value == ptr->value) ? 'N' : 'Y';
    char enabled = (ptr->enable) ? 'Y' : 'N';
    printf("%-10d%-32s%-16d%-16d%-8c%-8c\n", ptr->NO, ptr->expression,
           ptr->old_value, ptr->value, changed, enabled);
  }
}
