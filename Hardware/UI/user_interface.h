#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#define PAGE             0
#define FUNCTION         1
#define VALUE            2

#define FLOAT            0
#define INT              1

#define NAME_SIZE        30
#define STACK_SIZE       8
#define SCREEN_HEIGHT    LCD_H       //屏幕高度
#define SCREEN_WIDTH     LCD_W        //屏幕宽度
#define ITEM_NUM         7      //一页的最多item数
#define FONT_HEIGHT      16
#define FONT_WIDTH       16
#define PADDING_TITLE    1       //与标题的间距
#include "lcd_init.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"


typedef struct Value
{
    unsigned char   type;
    void            *value;
    float           increasement;
    int             increasement_int;
}Value;

struct PageStruct;
typedef struct Item
{
    char name[NAME_SIZE];
    unsigned char id;       //在所属页面的第几个
    unsigned char padding;
    unsigned char pointer; //定义指向的是什么类型  VALUE FUNCTION PAGE
    union{                  //选参数调节器，函数调用，页面显示
        Value data;
        void (*fun)();
        struct PageStruct *point_page;
    }func;
}Item;

typedef Item *Items[ITEM_NUM];
struct PageStruct{
    char name[NAME_SIZE];   //page的名字
    Items list;             //存储page内的所有item
    unsigned char count;    //page现有的item个数
};

typedef struct PageStruct *Pages[STACK_SIZE];
typedef struct PageStruct Page;
typedef struct Page_stack 
{
    Pages page;
    unsigned char size;
    /* data */
}Page_stack;

typedef void (*FUNC)(void);

extern Page page1;


void set_item_value_float(Item *item,float *value,float incres,char *name);
void set_item_value_int(Item *item,int *value,int incres,char *name);
void set_item_function(Item *item,FUNC funciton,char *name);
void set_item_indexpage(Item *item,Page *page,char *name);
void init_page(Page *,char *name);
void page_add_item(Page *,Item *);//给页面添加item
void set_base_page(Page *);//设置主页
void set_show_area(unsigned char left_top_x,unsigned char left_top_y,unsigned char right_bottm_x,unsigned char right_bottm_y);
void UI_TASK(void); //UI任务
void up(void);//上
void down(void);//下
void enter(void);//进入
void choose_or_cancel(void);//确认/取消
void back(void);//返回


//自定义 设置item要显示的内容
char *item_content(Item *item);
void init_ui(void);

#endif
