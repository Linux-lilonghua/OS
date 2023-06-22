/**
文件系统
*/
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

const int BLOCK_SIZE = 512; // 磁盘块大小
const int BLOCK_NUM = 1024; // 磁盘块数量
const int MAX_FILE_NUM = 128; // 最大文件数量
const int MAX_FILE_NAME_LEN = 32; // 最大文件名长度
const int MAX_FILE_SIZE = 1024 * 1024; // 最大文件大小

//磁盘类，管理磁盘操作
class Disk {
public:
    //  构造函数
    Disk() {
        data = new char[BLOCK_SIZE * BLOCK_NUM];
        memset(data, 0, BLOCK_SIZE * BLOCK_NUM);//初始化整个磁盘卷
    }
    //  析构函数
    ~Disk() {
        delete[] data;
    }
    //  从磁盘块读出数据
    void read(int block, char* buf) {
        memcpy(buf, data + block * BLOCK_SIZE, BLOCK_SIZE);
    }
    // 将数据写入磁盘块
    void write(int block, char* buf) {
        memcpy(data + block * BLOCK_SIZE, buf, BLOCK_SIZE);
    }

private:
    char* data;       //定义整个磁盘卷
};

//文件系统类，管理文件和目录操作
class FileSystem {
public:
    FileSystem() {
        disk = new Disk();                //初始化整个磁盘卷
        memset(fat, -1, sizeof(fat));     //初始化为-1，即整个磁盘卷都是空的
        memset(files, 0, sizeof(files));  //整个文件系统为空，不含有任何文件
        root = new Directory();	          //根目录
        current_dir = root;	              //当前目录等于根目录
    }

    ~FileSystem() {
        delete disk;
        delete root;
    }

    //格式化磁盘卷
    void format() {
        memset(fat, -1, sizeof(fat));
        memset(files, 0, sizeof(files));
        root->clear();
        current_dir = root;
    }

    //列出当前目录下的所有文件
    void ls() {
        vector<File*> files = current_dir->getFiles();
        for (int i = 0; i < files.size(); i++) {
            cout << files[i]->getName() << endl;
        }
        vector<Directory*> dirs = current_dir->getDirectorys();
        for (int i = 0; i < dirs.size(); i++) {
            cout << dirs[i]->getName() << endl;
        }

    }

    //进入目录
    void cd(char* name) {
        if (strcmp(name, "..") == 0) {
            if (current_dir != root) {
                current_dir = current_dir->getParent();
            }
        } else {
            Directory* dir = current_dir->getDirectory(name);
            if (dir != NULL) {
                current_dir = dir;
            } else {
                cout << "Directory not found" << endl;
            }
        }
    }

    //创建目录
    void mkdir(char* name) {
        Directory* dir = current_dir->getDirectory(name);
        if (dir != NULL) {
            cout << "Directory already exists" << endl;
        } else {
            dir = new Directory(name, current_dir);
            current_dir->addDirectory(dir);
        }
    }

    //创建文件
    void create(char* name) {
        if (current_dir->getFile(name) != NULL) {
            cout << "File already exists" << endl;
            return;
        }
        int block = findFreeBlock();
        if (block == -1) {
            cout << "No free block" << endl;
            return;
        }
        File* file = new File(name, block);
        current_dir->addFile(file);
        fat[block] = -2;			//-2表示文件的结束标志 //更新FAT表
        files[block] = file;
    }

    //数据写入文件
    void write(char* name, char* data, int size) {
        File* file = current_dir->getFile(name);
        if (file == NULL) {
            cout << "File not found" << endl;
            return;
        }
        int block = file->getBlock();
        int offset = 0;							//偏移量
		
		//关键部分
        while (size > 0) {
            char buf[BLOCK_SIZE];
            disk->read(block, buf);
            int n = min(size, BLOCK_SIZE);
            memcpy(buf, data + offset, n);
            disk->write(block, buf);
            offset += n;							//更新偏移量
            size -= n;
            if (size > 0) {							//先取出来，在存到磁盘里面
                int next = findFreeBlock();
                if (next == -1) {
                    cout << "No free block" << endl;
                    return;
                }
                fat[block] = next;
                block = next;
                fat[block] = -2;
            }
        }
        file->setSize(offset);
    }

    //从文件读出数据
    void read(char* name, char* buf, int size) {
        File* file = current_dir->getFile(name);
        if (file == NULL) {
            cout << "File not found" << endl;
            return;
        }
        int block = file->getBlock();
        int offset = 0;
        while (size > 0) {
            char data[BLOCK_SIZE];
            disk->read(block, data);
            int n = min(size, BLOCK_SIZE);
            memcpy(buf + offset, data, n);
            offset += n;
            size -= n;
            if (size > 0) {
                block = fat[block];
                if (block == -1) {
                    cout << "Read error" << endl;
                    return;
                }
            }
        }
    }

private:
    //文件类
    class File {
    public:
        File(char* name, int block) {
            strncpy(this->name, name, MAX_FILE_NAME_LEN);
            this->block = block;
            size = 0;
        }

        ~File() {}

		//得到文件名
        char* getName() {
            return name;
        }
		//得到文件块
        int getBlock() {
            return block;
        }
		//得到文件大小
        int getSize() {
            return size;
        }
		//设置文件大小
        void setSize(int size) {
            this->size = size;
        }

    private:
        char name[MAX_FILE_NAME_LEN];       //名称
        int block;                          //占用的磁盘块
        int size;                           //大小
    };

    //目录类
    class Directory {
    public:
        Directory() {
            name[0] = '/';
            name[1] = '\0';
            parent = NULL;
        }

        Directory(char* name, Directory* parent) {
            strncpy(this->name, name, MAX_FILE_NAME_LEN);
            this->parent = parent;
        }

        ~Directory() {
            for (int i = 0; i < files.size(); i++) {
                delete files[i];
            }
            for (int i = 0; i < dirs.size(); i++) {
                delete dirs[i];
            }
        }
		
		//得到目录名
        char* getName() {
            return name;
        }
		
		//得到父目录
        Directory* getParent() {
            return parent;
        }
		//添加文件
        void addFile(File* file) {
            files.push_back(file);
        }
		//添加目录
        void addDirectory(Directory* dir) {
            dirs.push_back(dir);
        }
		//得到所有文件
        File* getFile(char* name) {
            for (int i = 0; i < files.size(); i++) {
                if (strcmp(files[i]->getName(), name) == 0) {
                    return files[i];
                }
            }
            return NULL;
        }
		//根据名称返回目录
        Directory* getDirectory(char* name) {
            for (int i = 0; i < dirs.size(); i++) {
                if (strcmp(dirs[i]->getName(), name) == 0) {
                    return dirs[i];
                }
            }
            return NULL;
        }
		//得到所有目录
        vector<File*> getFiles() {
            return files;
        }
		//得到所有目录
        vector<Directory*> getDirectorys() {
            return dirs;
        }

        void clear() {
            files.clear();
            dirs.clear();
        }

    private:
        char name[MAX_FILE_NAME_LEN];//名称
        Directory* parent;			 //父目录
        vector<File*> files;		 //含有的文件项
        vector<Directory*> dirs;	 //含有的目录项
    };

    //文件系统的私有属性
    Disk* disk;
    int fat[BLOCK_NUM];	    //FAT表用于记录整个磁盘卷的使用情况
    File* files[BLOCK_NUM]; //存放文件系统的所有文件
    Directory* root;		//根目录
    Directory* current_dir;	//当前目录

    //查找空闲的磁盘块
    int findFreeBlock() {
        for (int i = 0; i < BLOCK_NUM; i++) {
            if (fat[i] == -1) {
                return i;
            }
        }
        return -1;
    }
};

int main() {
    FileSystem fs;			 //文件系统
    char cmd[256];			 //命令参数
    while (true) {
        cout << "> ";
        cin.getline(cmd, 256);
        char* p = strtok(cmd, " ");			//设置标记符
        if (strcmp(p, "ls") == 0) {
            fs.ls();
        } else if (strcmp(p, "cd") == 0) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                fs.cd(p);
            } else {
                cout << "Usage: cd <directory>" << endl;
            }
        } else if (strcmp(p, "mkdir") == 0) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                fs.mkdir(p);
            } else {
                cout << "Usage: mkdir <directory>" << endl;
            }
        } else if (strcmp(p, "create") == 0) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                fs.create(p);
            } else {
                cout << "Usage: create <file>" << endl;
            }
        } else if (strcmp(p, "write") == 0) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                char* data = strtok(NULL, "");
                if (data != NULL) {
                    fs.write(p, data, strlen(data));
                } else {
                    cout << "Usage: write <file> <data>" << endl;
                }
            } else {
                cout << "Usage: write <file> <data>" << endl;
            }
        } else if (strcmp(p, "read") == 0) {
            p = strtok(NULL, " ");
            if (p != NULL) {
                int size = 0;
                char* data = strtok(NULL, "");
                if (data != NULL) {
                    size = atoi(data);
                }
                char buf[MAX_FILE_SIZE];
                fs.read(p, buf, size);
                cout << buf << endl;
            } else {
                cout << "Usage: read <file> [size]" << endl;
            }
        } else if (strcmp(p, "format") == 0) {
            fs.format();
        } else if (strcmp(p, "exit") == 0) {
            break;
        } else {
            cout << "Unknown command" << endl;
        }
    }
    return 0;
}
