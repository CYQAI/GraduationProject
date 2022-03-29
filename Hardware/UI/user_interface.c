#include "user_interface.h"
#include "lcd.h"
#include "KEY.h"
#include "main.h"
Page *main_page;
Page *last_page;
//
Page_stack page_stack={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0};
unsigned char first_in=1;//第一次加载
unsigned char select_id=0;//当前选择id
unsigned char enter_flag=0;//
unsigned char back_flag=0;
unsigned char choose_or_cancel_flag=0;
unsigned char value_set_flag=0; //标志该操作是改变变量操作
unsigned char up_flag=0;
unsigned char down_flag=0;
unsigned char left_top_x=0;
unsigned char left_top_y=0;
unsigned char right_bottom_x=0;
unsigned char right_bottom_y=0;
float *convert_float;
int *convert_int;

void screen_clear(void)
{
    LCD_Fill(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,WHITE);
}

void  screen_draw_txt(unsigned char x,unsigned char y,char *string)
{
    /*避免警告*/
    uint8_t *str;
    str = (uint8_t *)string;
    /*根据ascll的规则，判断中英文*/
    if(*str >127){
        LCD_ShowChinese(x,y,str,RED,WHITE,16,0);
    }else{
        LCD_ShowString(x,y,str,RED,WHITE,16,0);
    }
}

void push(Page * elem) 
{
    if(page_stack.size < STACK_SIZE)
    {
        //page_stack.page[page_stack.size++]=elem;
        page_stack.page[page_stack.size++]=elem;
    }
}

Page * pop()
{
    if(page_stack.size>0){
        return page_stack.page[--page_stack.size];
    }
    return main_page;
}

//初始化页面
void init_page(Page * page,char *name)
{
    page->count=0;
    strcpy(page->name,name);
}

//将item设置成float型参数
void set_item_value_float(Item *item,float *value,float incres,char *name)
{
    item->func.data.value=(void*)value;
    item->func.data.type=FLOAT;
    item->func.data.increasement=incres;
    item->pointer=VALUE;
    strcpy(item->name,name);
}

void set_item_value_int(Item *item,int *value,int incres,char *name)
{
    item->func.data.value=(void*)value;
    item->func.data.type=INT;
    item->func.data.increasement_int=incres;
    item->pointer=VALUE;
    strcpy(item->name,name);
}

void set_item_function(Item *item,FUNC funciton,char *name)
{
    item->func.fun=funciton;
    item->pointer=FUNCTION;
    strcpy(item->name,name);

}

void set_item_indexpage(Item *item,Page *page,char *name)
{
    item->func.point_page=page;
    item->pointer=PAGE;
    strcpy(item->name,name);
}


// void uiprintf(const char *fmt,...)
// {
//     va_list vars;
//     static unsigned char x=0;
//     static unsigned char y=0;
//     char temp[8];
//     va_start(fmt,vars);
//     vsprintf(temp,fmt,vars);
//     screen_draw_txt(x,y,temp);
//     y++;
// }

//给page添加item
void page_add_item(Page * page,Item * item)
{
    if(page->count < ITEM_NUM)
    {
        page->list[page->count]=item;
        item->id=page->count;
        page->count=page->count+1;

    }
}
void set_base_page(Page *page)
{
    main_page=page;
}

/*
 *设置显示区域，传入两个点，代表矩形
 */
void set_show_area(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
    left_top_x=x1;
    left_top_y=y1;
    right_bottom_x=x2;
    right_bottom_y=y2;
}

/*
 *显示标题
 */
void draw_title()
{

     /*根据ascll的规则，判断中英文*/
    if(*main_page->name >127){
        unsigned char x=left_top_x+(right_bottom_x-left_top_x-(strlen(main_page->name)/3)*FONT_WIDTH)/2;
        screen_draw_txt(x,left_top_y,main_page->name);
    }else{
        unsigned char x=left_top_x+(right_bottom_x-left_top_x-strlen(main_page->name)*FONT_WIDTH)/2;
        screen_draw_txt(x,left_top_y,main_page->name);
    }
}

/*
 *显示item的name
 */
void update_item(Item *item)
{
    unsigned char y=left_top_y+FONT_HEIGHT*(item->id+PADDING_TITLE);
    char *str=item_content(item);
    //short margin=right_bottom_x-left_top_x-strlen(str)*FONT_WIDTH;
    screen_draw_txt(left_top_x+(FONT_WIDTH/2),y,str);
}

/*
 *显示page
 */
void draw_list()
{
    unsigned char i=0;
    screen_clear();

    draw_title();
    for(;i<main_page->count;i++)
        update_item(main_page->list[i]);
}

void (onItemSelect)(unsigned char item_id)
{
    unsigned char i=0;
    for(i=0;i<ITEM_NUM;i++)
    {
        if(item_id==i){
            screen_draw_txt(left_top_x,left_top_y+(PADDING_TITLE+i)*FONT_HEIGHT,">");
        }else
        {
            screen_draw_txt(left_top_x,left_top_y+(PADDING_TITLE+i)*FONT_HEIGHT," ");
        }
    }
}

void (onItemHover)(unsigned char item_id)
{
    unsigned char i=0;
    for(i=0;i<ITEM_NUM;i++)
    {
        if(item_id==i){
            screen_draw_txt(left_top_x,left_top_y+(PADDING_TITLE+i)*FONT_HEIGHT,"*");
        }else
        {
           screen_draw_txt(left_top_x,left_top_y+(PADDING_TITLE+i)*FONT_HEIGHT," ");
        }
    }
}

void up(void)
{
    up_flag=1;
}
void down(void)
{
    down_flag=1;
}
void enter(void)
{
    enter_flag=1;
}
void choose_or_cancel(void)
{
    choose_or_cancel_flag=!choose_or_cancel_flag;
}
void back(void)
{
    back_flag=1;
}

void key_ctrl(void)
{

    if (!(key.num>='A' && key.num<='D'))
        return;
    

    if (key.num  == 'A'){
        up();
    }
    else if (key.num  == 'B'){
        down();
    }
    else if (key.num  == 'C'){
        enter();
    }
    else if(key.num  == 'D'){
        back();
    }

}


void UI_TASK(void)
{
    if (task_status !=  run_ui_task)
        return;

	if(first_in){
		first_in=0;
		draw_list();
	}

    key_ctrl();

    if(!choose_or_cancel_flag) onItemHover(select_id); //悬停效果

    if(enter_flag)
    {
        switch (main_page->list[select_id]->pointer)
        {
            case PAGE:
            {
               
                if(main_page->list[select_id]->func.point_page!=NULL)
                {
                     push(main_page);//入栈 用来返回
                     main_page=main_page->list[select_id]->func.point_page; //进入新页面
                }
                draw_list();//进入新页面重新画一下
                select_id=0;
                break;
            }
            case FUNCTION:
            {
                if(main_page->list[select_id]->func.fun!=NULL)
                {
                    main_page->list[select_id]->func.fun();
                }
                //提示执行成功
                break;
            }
            case VALUE:break;
        }
        enter_flag=0;
    }
    else if(back_flag)
    {
        main_page=pop();//出栈 返回旧页面
        draw_list();//返回旧页面重新画一下
        back_flag=0;
    }
    else if(choose_or_cancel_flag)
    {
        if(choose_or_cancel_flag==1)
        {
            onItemSelect(select_id);//选择效果
            if(main_page->list[select_id]->pointer==VALUE)
            {
                value_set_flag=1;//设置变量
            }
        }
    }
    else if(!choose_or_cancel_flag)
    {
        value_set_flag=0;
    }

    if(up_flag)
    {
        up_flag=0;
        if(value_set_flag)
        {
            //增加变量
            if(main_page->list[select_id]->func.data.type==FLOAT)
            {
                convert_float=(float*)(main_page->list[select_id]->func.data.value);
                *convert_float=*convert_float + main_page->list[select_id]->func.data.increasement;
            }else if(main_page->list[select_id]->func.data.type==INT)
            {
                convert_int=(int*)(main_page->list[select_id]->func.data.value);
                *convert_int=*convert_int + main_page->list[select_id]->func.data.increasement_int;
            }
            update_item(main_page->list[select_id]);//重画更新的item
        }
        else
        {
            select_id++;
            if(select_id>=ITEM_NUM) select_id=0;
        }
    }
    else if(down_flag)
    {
        down_flag=0;
        if(value_set_flag)
        {
            //减小变量
             if(main_page->list[select_id]->func.data.type==FLOAT){
                convert_float=(float*)(main_page->list[select_id]->func.data.value);
                *convert_float=*convert_float - main_page->list[select_id]->func.data.increasement;
            }else if(main_page->list[select_id]->func.data.type==INT){
                convert_int=(int*)(main_page->list[select_id]->func.data.value);
                *convert_int=*convert_int- main_page->list[select_id]->func.data.increasement_int;
            }
            update_item(main_page->list[select_id]);//重画更新的item
        }
        else
        {
			if(select_id==0)select_id=ITEM_NUM-1;
            else select_id--;
            
        }
    }
}
