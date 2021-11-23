#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class node{
friend class addressbook;
private:
    char name[40];
    char phone[40];
    char email[40];
    node *left;
    node *right;
};

class addressbook{
public:
    addressbook();
    ~addressbook();
    void Insert();
    void Find(char *find_name);
    void Delete(char *find_name);
    void Print();

    //檔案處理
    void Login(char *file_name);
    void Logout();
    void Read(char *file_name);
    void Save(char *file_name);

private:
    void release(node *ptr);
    node *find_node(char *find_name);
    node *find_parent(char *find_name, int *pos);
    void print_orderbyname(node *ptr);
    void save_orderbyname(node *ptr);

    node *root;
    FILE *file;
    int n;
    char read_content[40];
};

addressbook::addressbook(){
    root = NULL;
}

addressbook::~addressbook(){
    release(root);
}

void addressbook::release(node *ptr){
    if(ptr!=NULL){
        release(ptr->left);
        release(ptr->right);
        free(ptr);
    }
}

void addressbook::Insert(){
    node *new_node;
    node *ptr;
    node *parent;

    new_node = (node *)malloc(sizeof(node));
    printf("姓名: ");
    scanf("%s", new_node->name);
    printf("電話: ");
    scanf("%s", new_node->phone);
    printf("信箱: ");
    scanf("%s", new_node->email);
    new_node->left = NULL;
    new_node->right = NULL;

    if(root==NULL){
        root = new_node;
    }else{
        ptr = root;
        while(ptr!=NULL){
            parent = ptr;
            if(strcmp(ptr->name, new_node->name) > 0)
                ptr = parent->left;
            else
                ptr = parent->right;
        }

        if(strcmp(parent->name, new_node->name) > 0)
            parent->left = new_node;
        else
            parent->right = new_node;
    }
    n++; //紀錄資料筆數
    printf("新增成功!\n");
}

node* addressbook::find_node(char *find_name){
    node *ptr;

    ptr = root;
    while(ptr!=NULL){
        if(strcmp(ptr->name, find_name) == 0)
            return ptr;
        else{
            if(strcmp(ptr->name, find_name) > 0)
                ptr = ptr->left;
            else
                ptr = ptr->right;
        }
    }

    return NULL;
}

void addressbook::Find(char *find_name){
    node *ptr;

    if((ptr = find_node(find_name))!=NULL){
        printf("搜尋結果:\n");
        printf("%s\n", ptr->name);
        printf("%s\n", ptr->phone);
        printf("%s\n", ptr->email);
    }
    else{
        printf("搜尋結果:\n");
        printf("無\n");
    }
}

node *addressbook::find_parent(char *find_name, int *pos){
    node *ptr;
    node *parent;

    ptr = parent = root; //find from root
    *pos = 0;
    while(ptr!=NULL){
        //parent = ptr; **bug**
        if(strcmp(ptr->name, find_name) == 0)
            return parent;
        else{
            parent = ptr;
            if(strcmp(ptr->name, find_name) > 0){
                ptr = parent->left;
                *pos = -1;
            }
            else{
                ptr = parent->right;
                *pos = 1;
            }
        }
    }

    return NULL;
}

void addressbook::Delete(char *find_name){
    node *ptr;
    node *parent;
    node *substitude;
    int pos;

    parent = find_parent(find_name, &pos);
    if(parent!=NULL){

        switch(pos){
            case 0:
                ptr = parent;
                break;
            case -1:
                ptr = parent->left;
                break;
            case 1:
                ptr = parent->right;
                break;
        }

        if(ptr->left==NULL){
            if(ptr==parent)
                root = root->right;
            else{
                if(pos==-1)
                    parent->left = ptr->right;
                else
                    parent->right = ptr->right;
            }
            free(ptr);
        }
        else if(ptr->right==NULL){
            if(ptr==parent)
                root = root->left;
            else{
                if(pos==-1)
                    parent->left = ptr->left;
                else
                    parent->right = ptr->left;
            }
            free(ptr);
        }
        else{
            parent = ptr;
            substitude = ptr->left;

            if(substitude->right!=NULL){
                while(substitude->right!=NULL){
                    parent = substitude;
                    substitude = substitude->right;
                }
                parent->right = substitude->left;
            }
            else
                ptr->left = substitude->left;

            strcpy(ptr->name, substitude->name);
            strcpy(ptr->phone, substitude->phone);
            strcpy(ptr->email, substitude->email);
            free(substitude);
        }
        n--; //紀錄資料筆數
        printf("刪除成功\n");
    }
    else{
        printf("無法刪除\n");
    }
}

void addressbook::print_orderbyname(node *ptr){
    if(ptr!=NULL){
        print_orderbyname(ptr->left);
        printf("姓名: ");
        printf("%s\n", ptr->name);
        printf("電話: ");
        printf("%s\n", ptr->phone);
        printf("信箱: ");
        printf("%s\n", ptr->email);
        printf("==========\n");
        print_orderbyname(ptr->right);

    }
}

void addressbook::Print(){
    printf("總共 %d 位聯絡人\n", n);
    print_orderbyname(root);
}

//帳號登入、登出
void addressbook::Login(char *file_name){
    file = fopen(file_name, "r");
    if(file==NULL){
        printf("帳號不存在，第一次使用\n");
        n = 0;
    }
    else{
        printf("登入成功\n");
        Read(file_name);
    }
}

void addressbook::Logout(){
    fclose(file);
    printf("帳號登出\n");
}

//檔案讀取、存檔
void addressbook::Read(char *file_name){
    node *new_node;
    node *ptr;
    node *parent;

    //讀取第一行得n筆資料
    fscanf(file, "%d\n", &n);

    //讀取n筆聯絡人資料，逐筆建樹，右歪斜樹
    for(int i=0;i<n;i++){
        new_node = (node *)malloc(sizeof(node));
        fscanf(file, "%s", new_node->name);
        //預防變數n錯誤，造成的讀取錯誤
        if(feof(file))
            break;
        fscanf(file, "%s", new_node->phone);
        fscanf(file, "%s", new_node->email);
        new_node->left = NULL;
        new_node->right = NULL;

        if(root==NULL){
            root = new_node;
        }else{
            ptr = root;
            while(ptr!=NULL){
                parent = ptr;
                if(strcmp(ptr->name, new_node->name) > 0)
                    ptr = ptr->left;
                else
                    ptr = ptr->right;
            }

            if(strcmp(parent->name, new_node->name) > 0)
                parent->left = new_node;
            else
                parent->right = new_node;
        }
    }
}

void addressbook::save_orderbyname(node *ptr){
    if(ptr!=NULL){
        save_orderbyname(ptr->left);
        fprintf(file, "%s\n", ptr->name);
        fprintf(file, "%s\n", ptr->phone);
        fprintf(file, "%s\n", ptr->email);
        save_orderbyname(ptr->right);
    }
}

void addressbook::Save(char *file_name){
    file = fopen(file_name, "w");
    fprintf(file, "%d\n", n);
    save_orderbyname(root);
    printf("已存檔!\n");
}

int main(void){
    char file_name[40];
    char op;
    addressbook ADB;
    char addressbook_name[40];

    printf("=== 歡迎使用通訊錄 ===\n");
    printf("請輸入您的帳號: ");
    scanf("%s", file_name);
    ADB.Login(file_name);

    //operate
    while(1){
        printf("<< %s 的通訊錄 >>\n", file_name);
        printf("i: 新增\n");
        printf("f: 尋找\n");
        printf("d: 刪除\n");
        printf("l: 查看所有\n");
        printf("s: 儲存修改\n");
        printf("q: 關閉/重新登入\n");
        printf(">");

        scanf(" %c", &op);
        switch(op){
        case 'i':
            ADB.Insert();
            break;
        case 'f':
            printf("輸入要尋找的聯絡人: ");
            scanf("%s", addressbook_name);
            ADB.Find(addressbook_name);
            break;
        case 'd':
            printf("輸入要刪除的聯絡人: ");
            scanf("%s", addressbook_name);
            ADB.Delete(addressbook_name);
            break;
        case 'l':
            ADB.Print();
            break;
        case 's':
            ADB.Save(file_name);
            break;
        case 'q':
            ADB.Logout();
            printf("(1)關閉 (2)重新登入 ");
            scanf(" %c", &op);
            if(op=='1')
                return 0;
            else{
                system("cls");
                main();
            }
            break;
        default:
            printf("操作錯誤!\n");
            break;
        }
        system("pause");
        system("cls");
    }

    return 0;
}
