#ifndef _MY_LIST_H_
#define _MY_LIST_H_

#ifdef _MY_LIST_C_
#define _MY_LIST_C_EXT_
#else
#define _MY_LIST_C_EXT_ extern
#endif

//#define container_of(ptr, type, member) ({ const typeof( ((type *)0)->member ) *__mptr = (ptr); \ 
//											(type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(type, member)			((unsigned long) &((type *)0)-> member)
#define container_of(ptr, type, member) (type *)((char *)ptr - offsetof(type,member))
#define prefetch(x) (x)



struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

//定义并初始化
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)	

//获取结构体指针
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

//获取ptr的下一个结构体
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

//===================  遍历链表 ======================//
//遍历链表
#define list_for_each(pos, head) \
	for (pos = (head)->next; prefetch(pos->next), pos != (head); \
        	pos = pos->next)

//没有预读取遍历链表
#define __list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

//从尾部遍历链表
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; prefetch(pos->prev), pos != (head); \
        	pos = pos->prev)

//安全遍历链表
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

//从尾部安全遍历链表
#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     prefetch(pos->prev), pos != (head); \
	     pos = n, n = pos->prev)

//===================  遍历结构体 ======================//
//遍历结构体		 
#define list_for_each_entry(pos, head, member, type)			\
	for (pos = list_entry((head)->next, type, member);	\
	     prefetch(pos->member.next), &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))

//安全遍历结构体
#define list_for_each_entry_safe(pos, n, head, member, type)	\
	for (pos = list_entry((head)->next, type, member),	\
		n = list_entry(pos->member.next, type, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, type, member))	
		 
//反向遍历结构体		 
#define list_for_each_entry_reverse(pos, head, member, type)	\
	for (pos = list_entry((head)->prev, type, member);	\
	     prefetch(pos->member.prev), &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, type, member))

//安全反向遍历结构体	 
#define list_for_each_entry_safe_reverse(pos, n, head, member, type) \
	for (pos = list_entry((head)->prev, type, member),	\
		n = list_entry(pos->member.prev, type, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, type, member))
		 
		 
	
_MY_LIST_C_EXT_ void list_add(struct list_head *new, struct list_head *head);
_MY_LIST_C_EXT_ void list_add_tail(struct list_head *new, struct list_head *head);
_MY_LIST_C_EXT_ void list_del(struct list_head *entry);
_MY_LIST_C_EXT_ void list_del_init(struct list_head *entry);
_MY_LIST_C_EXT_ void list_replace(struct list_head *old,struct list_head *new);
_MY_LIST_C_EXT_ void list_replace_init(struct list_head *old,struct list_head *new);
_MY_LIST_C_EXT_ void list_move(struct list_head *list, struct list_head *head);
_MY_LIST_C_EXT_ void list_move_tail(struct list_head *list,struct list_head *head);


#endif

