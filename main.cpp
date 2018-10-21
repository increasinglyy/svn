#include<iostream>
#include<string>
#include<io.h>     //_access
#include<direct.h> //_mkdir, getcwd
#include<vector>
#include<fstream>  //ofstream
#include <sstream> //ostringstream
#include<time.h>   //time(0)
#include<algorithm>//min()
#include <cstring> //cout不明确
using namespace std;

#define WORK_PATH 80
void create(string repo_name);
void create_init(string svn_path);
void workingDir(string &filePath);

void status();
const string getLabel(const string str);
void getAllFiles(string path, vector<string>& files);
void getAllFilesInfo(string path, vector<string>& files);

void add(string file_name);
string readFileIntoString(const string filename);
void readFileIntoVector(const string filename, vector<string>& content);
void subFilename(vector<string>& file);
string subFilename(const string file);

void commit(string parameter);
void backup(const string versionNum, vector<string> add_file);
void backupD(const string versionNum, vector<string> commit_file);
void copyFile(string pszFileCopee, string pszFileCoper);
int diff2(const string f1, const string f2);
int diff(string file);
bool searchInManage(const string file);
void searchInNewVersion(const string file);

void mkDir(string path);
void writeVectorIntoFile(const string filename, vector<string> content);
void modifyLabelInFile(string filename, int lineNum, string label);

void del(string parameter);
void deleteContentInFile(string filename, int lineNum);
void modifiedContentInFile(string filename, int lineNum, string contentLine);

void move(string parameter);
void deleteOldPath(string& manage_str);
void deleteOldPath(string& manage_str, string& old_path);

void gitLog(string parameter);
void gitLogAll();
string getLogToString(const string filename);
void gitLogFile(const string filename);
string getFileInLog(const string filename);

void revert(string parameter);
void revertAll();
void revertFile(const string file);
void revertVersion(const string version);
void revertFileVersion(const string file, const string version);

vector<string> getVersions();
string getLastVersion();
string getLastVersion(vector<string> versions, string filename);
vector<string> getVersionContent(string version);
void revertA(string filename, string version_number);
string getLabelInManage(string filename);
string getWorkingPath(string filename);
string getNewVersionPath(string line);
void revertManageFile(string filename, string label);

bool cmp(string a, string b);
bool cmpFileInVersion(string version, string filename);

//目前只能在main.cpp文件目录下创建仓库.svn
int main()
{
	string command;//整条输入指令
	getline(cin, command);

	if (command.find("svn") != -1)
	{
		string function;//指令功能
		//"svn "后面的字符串，
		function = command.substr(command.find("svn ",0) + 4);//从0开始查找，返回svn 出现的位置，+4表示“svn ”之后的一位
		string func;// create...
		string parameter; // 参数（目录名文件名等）
		if (function.find(" ") != -1) //function中含有" "
		{
			func = function.substr(0, function.find_first_of(" "));//commit     aadd.txt -m 'x'
			string temp;
			temp = function.substr(function.find_first_of(" "));//    aadd.txt -m 'x'(多了空格)
			parameter = temp.substr(temp.find_first_not_of(" "));//aadd.txt -m 'x'
		}
		else
		{
			func = function;//status, diff...
		}

		//svn create [目录名]
		if (func == "create")
		{
			create(parameter);
		}
		//svn status
		else if (func == "status")
		{
			status();
		}
		//svn add [文件名/目录名]
		//svn add ffff\f1\ff1.txt
		else if (func == "add")
		{
			add(parameter);
		}
		//svn commit [文件名/目录名] (-m '提交日志')
		else if (func == "commit")
		{
			commit(parameter);
		}
		//svn delete [文件名] (--force)
		else if (func == "delete")
		{
			del(parameter);
		}
		//svn mv [文件名/目录名] [目标目录]
		else if (func == "mv")
		{
			move(parameter);
		}
		//svn git log (文件名)
		else if (func == "git")
		{
			gitLog(parameter);
		}
		else if (func == "revert")
		{
			revert(parameter);
		}
		else if (func == "test")
		{
			string s = "ffff\aadd.txt aadd.txt 1539693925 1539693934 MV";
			int line = 3;
			deleteOldPath(s);
		}
		else
		{
			cout << "fuction error, svn <function> [parameter]" << endl;
			cout << "function : create, status, add, commit, delete, mv, git log, revert" << endl;
		}
	}
	else
	{
		cout << "command error, svn <function> [parameter]" << endl;
	}

	system("pause");
	return 0;
}

//svn create [目录名]     svn create test
//初始化一个版本仓库，在当前目录下创建一个目录，目录内初始化一个.svn文件夹，（.svn文件夹下应该记录些东西）
//用于存储仓库版本数据，如果当前目录已在一个版本仓库下，创建失败
void create(string repo_name)
{
	string svn_path;
	if (repo_name.empty() == 1)//没有输出参数（仓库名）
	{
		//判断当前目录下是否 存在.svn文件夹（已在一个版本仓库下）
		if (_access(".svn", 0) == -1)//当前目录下不存在.svn
		{
			svn_path = ".svn";
			_mkdir(".svn");//当前目录下创建.svn文件

			//还要创建一系列文件
			create_init(svn_path);
		}
		else
			cout << "Repository exists, create fails" << endl;
	}
	else //仓库名不为空
	{
		_mkdir(repo_name.c_str());//创建仓库repo_name
		svn_path = repo_name + "\\.svn";
		if (_access(svn_path.c_str(), 0) == -1)
		{
			_mkdir(svn_path.c_str());
			create_init(svn_path);
		}
		else
			cout << "Repository exists, create fails" << endl;
	}
}

//创建.svn下的目录和记录文件，svn_path = ".svn";或者svn_path = repo_name + "//.svn";
void create_init(string svn_path)
{
	cout << "Create an empty Svn repository" << endl;
	//创建文件夹commit（版本信息）,copyfile备份文件
	_mkdir((svn_path + "\\commit").c_str());
	_mkdir((svn_path + "\\copyfile").c_str());
	_mkdir((svn_path + "\\copyfile\\newVersion").c_str());

	//创建文件manage_file（受管理文件的信息）
	ofstream manage;
	manage.open(svn_path + "\\manage_file", ios::out);//"manage_file"是文件名，以写入方式打开文件，双斜杠
	manage.close();
}

//获取当前工作目录
void workingDir(string &filePath)
{
	char   buffer[WORK_PATH];
	getcwd(buffer, WORK_PATH);//getcwd()会将当前工作目录的绝对路径复制到参数buffer所指的内存空间中,参数maxlen为buffer的空间大小。
	filePath.assign(buffer);//当前文件夹的路径，D:\cpp\svn\in_this_dir\in_this_dir
}

//svn status 查看当前仓库文件夹下有发生变化的文件
//‘+ b.cpp’表示新增的文件
void status()
{
	string working_path;
	workingDir(working_path);

	//获取工作目录下所有文件
	vector<string> files;//工作目录下所有文件的相对路径
	getAllFilesInfo(working_path, files);//files是当前文件
	
	//与manage_file中的文件进行比较，在manage_file中查找有无该文件，
	vector<string> manage_str; //manage_file文件中的内容
	readFileIntoVector(".svn\\manage_file", manage_str);
	//vector<string> managed_files;
	//vector<string> unmanaged_files;
	string filename;//工作目录下所有文件
	string manage_file;
	for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i)
	{
		filename = subFilename(*i);
		int j = 0;
		for (; j < manage_str.size(); ++j)
		{
			manage_file = subFilename(manage_str[j]);
			if (filename == manage_file)//filename在文件manage_file中找到，说明已被管理
			{
				string label = getLabel(manage_str[j]);
				if (label == "M")
				{
					cout << "M " << filename << endl;
				}
				if (label == "+")
				{
					cout << "+ " << filename << endl;
				}
				if (label == "D")
				{
					cout << "D " << filename << endl;
				}
				if (label == "MV")//MV d.php ./d.php -> newDIr/d.hp
				{
					//manage_file(filename)更改后的新地址，从manage_str[j]中读出原始地址
					string old_path = manage_str[j].substr(manage_str[j].find(" ") + 1);
					old_path = old_path.substr(0, old_path.find(" "));
					cout << "MV " << old_path << " .\\" << old_path << " -> .\\" << filename << endl;
					
				}
				if (label == "C")
				{
					//检查是否修改
					if (diff(filename))
					{
						modifyLabelInFile(".svn\\manage_file", j + 1, "M");
						//不同则修改label
						cout << "M " << filename << endl;
					}
					else
						cout << "C " << filename << endl;
				}
				break;
			}
		}
		if (j == manage_str.size())
			cout << "? " << filename << endl;
	}
}

//获得str的label
const string getLabel(const string str)
{
	return str.substr(str.find_last_of(" ") + 1);
}

//getFiles(filePath, files);
//获取当前文件夹下所有文件与文件夹, path为当前文件夹的全路径working_path
//添加了files相对路径
void getAllFiles(string path, vector<string>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	string working_dir;
	workingDir(working_dir);
	working_dir = working_dir + "\\";
	int len = working_dir.size();
	string temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//如果查找成功的话，将返回一个long型的唯一的查找用的句柄（就是一个唯一编号）。否则返回-1
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//atrrib：文件属性的存储位置。_A_SUBDIR（文件夹）
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".svn") != 0)
				{
					temp = p.assign(path).append("\\").append(fileinfo.name);
					temp = temp.substr(temp.find(working_dir, 0) + len);//去掉绝对路径
					//string create_time = to_string(fileinfo.time_create);//添加创建时间
					files.push_back(temp);
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				temp = p.assign(path).append("\\").append(fileinfo.name);
				temp = temp.substr(temp.find(working_dir, 0) + len);
				//string create_time = to_string(fileinfo.time_create);//添加创建时间
				files.push_back(temp);
			}
		} while (_findnext(hFile, &fileinfo) == 0);//若成功返回0，否则返回-1。
		_findclose(hFile);
	}
}

//getFiles(filePath, files);
//获取当前文件夹下所有文件与文件夹（包括创建和修改时间）, path为当前文件夹的全路径
void getAllFilesInfo(string path, vector<string>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	string working_dir;
	workingDir(working_dir);
	working_dir = working_dir + "\\";
	int len = working_dir.size();
	string temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//如果查找成功的话，将返回一个long型的唯一的查找用的句柄（就是一个唯一编号）。否则返回-1
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//atrrib：文件属性的存储位置。_A_SUBDIR（文件夹）
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".svn") != 0)
				{
					temp = p.assign(path).append("\\").append(fileinfo.name);
					temp = temp.substr(temp.find(working_dir, 0) + len);//去掉绝对路径
					string create_time = to_string(fileinfo.time_create);//添加创建时间
					string modified_time = to_string(fileinfo.time_write);//添加修改时间
					files.push_back(temp.append(" ").append(create_time)
										.append(" ").append(modified_time));
					getAllFilesInfo(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				temp = p.assign(path).append("\\").append(fileinfo.name);
				temp = temp.substr(temp.find(working_dir, 0) + len);
				string create_time = to_string(fileinfo.time_create);
				string modified_time = to_string(fileinfo.time_write);
				files.push_back(temp.append(" ").append(create_time)
									.append(" ").append(modified_time));
			}
		} while (_findnext(hFile, &fileinfo) == 0);//若成功返回0，否则返回-1。
		_findclose(hFile);
	}
}

//从文件读取信息到string里, ".svn//manage_file"
string readFileIntoString(const string filename)
{
	ifstream infile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf&&infile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

//从文件读取信息到vector<string>里, ".svn//manage_file"
void readFileIntoVector(const string filename, vector<string>& content)
{
	ifstream infile(filename);       //读入文件
	string line;
	while (getline(infile, line))   //按行读取
	{
		content.push_back(line);
	}
}

//将文件名（包括相对路径提取出来），去掉时间等其他信息
void subFilename(vector<string>& file)
{
	string temp;
	for (vector<string>::iterator i = file.begin(), end = file.end(); i != end; ++i)
	{
		temp = (*i);
		*i = temp.substr(0, temp.find(" ", 0));
	}
}
string subFilename(const string file)
{
	return file.substr(0, file.find(" ", 0));
}

//svn add [文件名/目录名] 
//将一个未受管理的文件添加到管理范畴，
//如果已被管理则无任何效果，注意与git add区分。（git add 是修改后就可以add）
void add(string file_name)
{
	if (file_name.empty() == false)
	{
		string working_path;//当前工作目录
		workingDir(working_path);
		vector<string> files;//工作目录下所有文件的相对路径
		getAllFilesInfo(working_path, files);//要找包括修改时间的信息
		string temp_file;
		int fileNum = 0;

		//遍历files，找要add的文件（file_name）是否已被托管（在manage_file）中
		for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i, ++fileNum)
		{
			//先判断要add的文件（file_name）是否在工作目录的文件中（files）
			temp_file = subFilename(*i);//工作目录的文件名
			if (temp_file == file_name)
			{
				if (file_name == temp_file)//如果遍历中找到要添加的文件/文件夹（输入文件名合法），再判断该文件是否已被托管、修改
				{
					vector<string> manage_vec;//读入manage_file到vector
					readFileIntoVector(".svn\\manage_file", manage_vec);
					//遍历manage_vec，找到file_name是否被托管
					for (vector<string>::iterator j = manage_vec.begin(), e = manage_vec.end(); j != e; ++j)
					{
						if (file_name == subFilename(*j))//已被托管，无法在add
						{
							cout << file_name << " is managed" << endl;
							return;
						}
					}
					//不在manage_file中或manage_file为空，则加入托管（manage_file）
					ofstream manage;
					manage.open(".svn\\manage_file", ios::app);//manage_file文件追加添加内容
					manage << (*i).append(" +") << endl;       //添加文件信息，在末尾加入+符合标记
					manage.close();
					cout << "add " << subFilename(*i) << " to repo" << endl;
					return; //结束遍历
				}
			}
		}
		if (fileNum == files.size())//遍历完所有目录下的文件
		{
			cout << file_name << " is not in directory" << endl;
			return;
		}
	}
	else
		cout << "add ???" << endl;
}


//每一次commit都会产生一个新的版本
//svn commit [文件名/目录名] (-m '提交日志')     和git的区别
//提交修改，记录当次提交修改的文件及其修改内容，同时保存日志，commit文件夹中
//如果提交成功，返回一个递增版本号，如果没有文件发生变化，提交失败，提示“no modification”
//为方便日后恢复，可在.svn 保存一份新版本文件的备份，copyfile文件夹中
///////////////////////////////////////////////////     MV 恢复newVersion中的路径   D 删除
void commit(string parameter)
{
	string filename;//文件名/目录名
	filename = parameter.substr(0, parameter.find_first_of(" ", 0));
	if (filename == "-m")//没有输入文件/目录名
	{
		cout << "Please inter file or directory name："<<endl
				<< "svn commit [文件名/目录名] (-m '提交日志')" << endl;
		return;
	}

	//查找文件名：1、是否受托管（在manage_file中），2、是否有有'C'
	vector<string> file; //读取manage_file文件的每行
	readFileIntoVector(".svn\\manage_file", file);


	int flag = 0;//标记文件是否受托管（manage_file中）
	int line = 1;
	vector<string> commit_before;   // 添加可以commit的文件，commit前的文件信息 （可以改用string，只会有一个）
	vector<string> commit_after;    // commit后的文件信息

	//对manage_file文件中的每行进行遍历，检查是否可以commit（标记+ M D MV），
	//可以则加入commit_file，并修改manage_file文件中的标签
	string old_label;//原有的标记
	for (vector<string>::iterator i = file.begin(), end = file.end(); i != end; ++i, ++line)
	{
		old_label = getLabel(*i);
		if (filename==subFilename(*i))  // 如果文件存在于仓库中
		{
			flag = 1;
			if (old_label == "C")
			{
				//之前commit过 检查是否修改，源文件和newVersion对比
				if (diff(filename))
				{
					modifyLabelInFile(".svn\\manage_file", line, "M");
					old_label = "M";
				}

			}
			if (old_label == "D")
			{
				deleteContentInFile(".svn\\manage_file", line);//如果是D，则删除manage_file中该文件
				commit_before.push_back(*i);
				//如果是要删除的文件，则没有commit之后的信息
			}
			if (old_label == "+" || old_label == "M" || old_label == "MV")
			{
				string temp;
				string old_path;//MV 中原来的path（包括文件名）
				temp = (*i);
				//commit之后 MV文件 要在newVerion中更改（删除原来的，添加新的 路径文件），修改manage_file为commit之后的
				if (old_label == "MV")
				{
					deleteOldPath(temp, old_path);//temp是新的文件信息，这个old_path是上一次（移动前）的路径
					deleteOldPath(temp, old_path);//                   这个old_path是newVersion的路径
					//删除newVersion中old path
					old_path = ".svn\\copyfile\\newVersion\\" + old_path;
					remove(old_path.c_str());
					//改manage_file 为temp
					modifiedContentInFile(".svn\\manage_file", line, temp);//修改某行内容
				}

				modifyLabelInFile(".svn\\manage_file", line, "C");

				commit_before.push_back(*i);      //加入符合条件的文件+ M D MV（加入原始的，存到commit版本文件中）
				temp = temp.substr(0, temp.find_last_of(" "));//去掉标签
				temp = temp.append(" C");
				commit_after.push_back(temp);
			}
		}
	}
	if (commit_before.empty() == false) //文件（修改的文件+ M D MV）不为空，提交修改
	{
		string log = "";//提交日志
		if (parameter.find("-m ") != -1)// parameter.find(" -m ")  4294967295相当于-1
		{
			log = parameter.substr(parameter.find(" -m ", 0) + 5);
			int len = log.size() - 1;
			log = log.substr(0, len);
		}

		//返回递增版本号（将当前时间作为递增版本号）
		time_t currentTime = time(0);
		string versionNum = to_string(currentTime);
		//创建记录文件（以版本递增号作为文件名）， 于commit目录下
		string commit_path = ".svn\\commit\\";
		ofstream version;
		version.open(commit_path + versionNum, ios::out);
		version << log << endl;//version文件写入提交日志log
		for (vector<string>::iterator i = commit_before.begin(), end = commit_before.end(); i != end; ++i)
		{
			version << (*i) << endl;//写入新添加的文件和创建修改时间
			if (old_label != "D")
				version << commit_after[0] << endl;
		}
		version.close();

		//备份
		if (old_label == "D")
			backupD(versionNum, commit_before);//删除newVersion下的文件
		else
			backup(versionNum, commit_before);

		cout << "commit success, version number is : " << versionNum << endl;
	}
	else    //commit_file is empty
	{
		if (flag == 1)//文件在仓库（但没有修改）
			cout << filename << " no modification" << endl;
		else
			cout << filename << " did not add to repo" << endl;
	}
}

//备份工作目录下的文件到 copyfile (newVersion和versionNum)
void backup(const string versionNum, vector<string> commit_file)
{
	//备份
	string copy_path = ".svn\\copyfile\\";
	_mkdir((copy_path + versionNum).c_str());//在copyfile下创建文件夹（以版本号为名）

	string source_file;//工作目录下的文件
	string copy_file;
	for (size_t i = 0; i < commit_file.size(); i++)
	{
		source_file = subFilename(commit_file[i]);
		
		string new_version_path = ".svn\\copyfile\\newVersion\\";
		string version_num_path = ".svn\\copyfile\\" + versionNum + "\\";

		//要备份的文件中含有\（子目录），要先创建子目录
		if (source_file.find("\\") != -1)
		{
			new_version_path = new_version_path + source_file.substr(0, source_file.find_last_of("\\")) + "\\";
			//cout << new_version_path;
			mkDir(new_version_path);
			version_num_path = version_num_path + source_file.substr(0, source_file.find_last_of("\\")) + "\\";
			mkDir(version_num_path);
			//_mkdir((".svn\\copyfile\\newVersion" + temp).c_str());
		}
		copy_file = source_file.substr(source_file.find_last_of("\\") + 1);

		copyFile(source_file, version_num_path  + copy_file);//将文件写入版本号文件夹
		copyFile(source_file, new_version_path  + copy_file); //将文件写入新版本文件夹（如果不同文件夹下有重名文件怎么办？？？）
	}
}
//备份删除的文件（versionNum)，删除newVersion下的文件
void backupD(const string versionNum, vector<string> commit_file)
{
	string copy_path = ".svn\\copyfile\\";
	_mkdir((copy_path + versionNum).c_str());//在copyfile下创建文件夹（以版本号为名）

	string source_file;//工作目录下的文件
	string copy_file;
	for (size_t i = 0; i < commit_file.size(); i++)
	{
		source_file = subFilename(commit_file[i]);

		string version_num_path = ".svn\\copyfile\\" + versionNum + "\\";//backup
		string new_version_path = ".svn\\copyfile\\newVersion\\" + source_file;//delete

		//要备份的文件中含有\（子目录），要先创建子目录
		if (source_file.find("\\") != -1)
		{
			version_num_path = version_num_path + source_file.substr(0, source_file.find_last_of("\\")) + "\\";
			mkDir(version_num_path);
		}
		copy_file = source_file.substr(source_file.find_last_of("\\") + 1);
		copyFile(new_version_path, version_num_path + copy_file);//将文件写入版本号文件夹
		remove(new_version_path.c_str());
	}
}


//拷贝文件，copyFile("f:/info", "f:/coper");
void copyFile(string pszFileCopee, string pszFileCoper)
{
	fstream fsCopee(pszFileCopee, ios::binary | ios::in);
	fstream fsCoper(pszFileCoper, ios::binary | ios::out);
	fsCoper << fsCopee.rdbuf();
}

//识别两个文本的不同行，并生成一个新的临时文本 temp
//文本相应位置显示出两个源文本相同与不同之处，不同之处需标记处属于哪个源文件
//	string f1 = "info";string f2 = "copyer";传入两个文件的相对路径 ffff\f1.txt
int diff2(const string f1, const string f2)
{
	vector<string> v1, v2;
	readFileIntoVector(f1, v1);
	readFileIntoVector(f2, v2);

	int flag = 0;//表示两个文件相同
	ofstream temp; //写入文件
	temp.open(".svn\\temp", ios::out);
	decltype(v1.size()) i = 0;
	for (; i < min(v1.size(), v2.size()); ++i)
	{
		if (v1[i] != v2[i])
		{
			//cout << "different line: " << i + 1 << endl;//不同之处的行号
			//文本相应位置显示出两个源文本相同之处
			temp << "diff:" << f1 << ":" << v1[i] << "|||" << f2 << ":" << v2[i] << endl;
			flag = 1;
		}
		else
			temp << v1[i] << endl;
	}
	if (v1.size() > v2.size())//f1文本长度大于f2
	{
		while (i<v1.size())
		{
			temp << "diff:" << f1 << ":" << v1[i] << endl;
			//cout <<"different line: " << ++i << endl;
			++i;
		}
		flag = 1;
	}
	else if (v1.size() < v2.size())
	{
		while (i<v2.size())
		{
			temp << "diff:" << f2 << ":" << v2[i] << endl;
			++i;
			//cout << ++i << endl;
		}
		flag = 1;
	}
	temp.close();
	return flag;//返回1表示两个文件不同
}

//查看已受托管的某个文件是否修改，传入文件在工作目录下的相对路径
//返回1表示两个文件不同
int diff(string file)
{
	if (searchInManage(file))//在托管中找到文件
	{
		string f1, f2;
		f1 = file;
		f2 = ".svn\\copyfile\\newVersion\\" + file;
		int flag = 0;//0表示文件相同
		flag = diff2(f1, f2);
		return flag;////返回1表示两个文件不同
	}
	else
	{
		cout << "this file did not been managed" << endl;
		return 0;
	}
}

//在manage_file中找到file，返回真
bool searchInManage(const string file)
{
	vector<string> files;
	readFileIntoVector(".svn\\manage_file", files);
	for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i)
	{
		if (subFilename(*i) == file)
			return true;
	}
}

//遍历新版本文件夹，找到file 返回文件路径
void searchInNewVersion(const string file)
{
	string new_version_path = "";
	string working_path;
	workingDir(working_path);
	new_version_path = working_path + "\\.svn\\copyfile\\newVersion";
	vector<string> files;//最新版本下的文件
	getAllFiles(new_version_path, files);
	if (files.empty() == false)
	{
		string result;
		for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i)
			//(*i).svn\copyfile\newVersion\ffff 1539521924
		{
			result = (*i).substr((*i).find(".svn\\copyfile\\newVersion\\")+25);
			result = result.substr(0, result.find(" "));
			cout << result << endl;
			if (result == file)//result	"ffff\\f1\\ff1.txt"
				cout << result << "xx" << endl;
		}
	}

}

//创建多层目录  path="test\\test1\\test2"
void mkDir(string path)
{
	int m = 0;
	string temp_path;
	while (m >= 0)
	{
		m = path.find("\\");//没有找到返回-1，找到返回第一个字符的索引
		temp_path += path.substr(0, m) + "\\";
		if (_access(temp_path.c_str(), 0) == -1)
			_mkdir(temp_path.c_str());
		if (m != -1)
			path = path.substr(m + 1, path.size());// "\\"+1,   "//"+2,  \\转义字符
	}
}

//将vector的内容写入文件
void writeVectorIntoFile(const string filename, vector<string> content)
{
	ofstream outfile;
	outfile.open(filename.c_str(), ios::out);
	for (vector<string>::iterator i = content.begin(), end = content.end(); i != end; ++i)
	{
		outfile << (*i) << endl;
	}
	outfile.close();
}

//修改manage_file每行之后的标签，filename要更改的文件 .svn\manage_file，lineNum要改的文件中的行号，label改后的标签
void modifyLabelInFile(string filename, int lineNum, string label)
{
	vector<string> content;
	readFileIntoVector(filename, content);//将文件读到vector中
	int index = lineNum - 1;//要修改的行
	string modify_line = content[index];//要修改的内容
	modify_line = modify_line.substr(0, modify_line.find_last_of(" "));//删除原有标签
	modify_line = modify_line.append(" ").append(label);//添加标签
	content[index] = modify_line;

	//vector写入文件
	writeVectorIntoFile(filename, content);
}


//svn delete [文件名] (--force)：将一个受管理的文件纳出管理状态（不删除物理文件）
//--->>将manage_file中该文件标记为D，（不删除newVersion中的信息，commit之后才删除）
//如果未被管理，则无任何效果，通过--force删除实际文件
void del(string parameter)
{
	string file;
	bool force = false;//默认不删除实际文件
	if (parameter.find(" --force") != -1)
	{
		force = true;
		file = parameter.substr(0, parameter.find(" "));//截取文件名
	}
	else
		file = parameter;
	//判断文件是否被管理 manage_file
	vector<string> manage_file;
	readFileIntoVector(".svn\\manage_file", manage_file);
	for (int i = 0; i < manage_file.size(); ++i)
	{
		if (subFilename(manage_file[i]) == file)//找到manage_file中的文件
		{
			string label = getLabel(manage_file[i]);
			if (label != "C" && label != "D")//标记为C的文件可以被delete
			{
				cout << label << " status can not been delete, you should commit before del" << endl;
				return;
			}
			//delete
			//deleteContentInFile(".svn\\manage_file", i + 1);
			modifyLabelInFile(".svn\\manage_file", i + 1, "D");
			//remove((".svn\\copyfile\\newVersion\\" + file).c_str());
			
			cout << file << " delete" << endl;
			if (force)
			{
				remove(file.c_str());
				cout << "and delete source file" << endl;
			}
			
			return;
		}
	}
	//未被管理
	cout << "delete fail, " << file << " did not been managed" << endl;
}

//删除文件中的某一行
void deleteContentInFile(string filename, int lineNum)
{
	vector<string> content;
	readFileIntoVector(filename, content);
	int index = lineNum - 1;
	for (; index < content.size() - 1; ++index)
	{
		content[index] = content[index + 1];
	}
	content.pop_back();
	writeVectorIntoFile(filename, content);
}
//在文件中修改一行
void modifiedContentInFile(string filename, int lineNum, string contentLine)
{
	vector<string> content;
	readFileIntoVector(filename, content);//读入文件到content
	content[lineNum - 1] = contentLine;//修改content
	writeVectorIntoFile(filename, content);//将content写入文件
}

//svn mv [文件名/目录名] [目标目录]
//移动文件（夹），注意更新仓库文件位置记录（manage_file）
//mv 改了多次，old_path 一直变，但仓库newVersion的path没变（要记录）
//ffff\aadd.txt a\aadd.txt a\aadd.txt 1539693925 1539693934 MV
//更改后的目录  源目录     newVersion目录
void move(string parameter)
{
	string source_file, goal_path ,filename;
	source_file = parameter.substr(0, parameter.find(" "));//源文件路径+名字
	goal_path = parameter.substr(parameter.find(" ") + 1);//目标目录
	if (source_file.find('\\') != -1)//源文件含有目录
		filename = source_file.substr(source_file.find("\\") + 1);
	else
		filename = source_file;
	

	vector<string> manage_file;
	string change_path;//更改后的路径
	readFileIntoVector(".svn\\manage_file", manage_file);
	
	for (int i = 0; i < manage_file.size(); ++i)
	{
		if (subFilename(manage_file[i]) == source_file)//在manage_file中找到要移动的文件
		{
			if (getLabel(manage_file[i]) == "MV" || getLabel(manage_file[i]) == "C")//只有标记为 MV 和 C的文件才能被mv
			{
				change_path = goal_path + "\\" + filename;
				//移动文件
				copyFile(source_file, change_path);
				remove(source_file.c_str());

				//更新仓库记录 path 标记MV
				if (getLabel(manage_file[i]) == "C") //保留newVersion下的路径
				{
					manage_file[i] = source_file + " " + manage_file[i];
				}
				if (getLabel(manage_file[i]) == "MV")
				{
					deleteOldPath(manage_file[i]);//如果原来文件就是MV 要删掉中间的路径
				}
				if (change_path.substr(0,2)==".\\")//如果新目录变更到当前目录下，  .\（去掉）
				{
					manage_file[i] = change_path.substr(change_path.find(".\\") + 2) + " " + manage_file[i]; 
				}
				else
					manage_file[i] = change_path + " " + manage_file[i]; //更新后的路径文件+原有的路径文件   
				manage_file[i] = manage_file[i].substr(0, manage_file[i].find_last_of(" "));//删除标记
				manage_file[i] = manage_file[i].append(" ").append("MV");//添加标记
				writeVectorIntoFile(".svn\\manage_file", manage_file);  //写入文件
				cout << "path changed: " << source_file << " -> " << change_path << endl;
				return;
			}
			else
			{
				cout << source_file << " did not been moved, you should commit this file." << endl;
				return;
			}
		}
	}
	cout << filename << " did not managed" << endl;
}

//////ffff\aadd.txt a\aadd.txt  1539693925 1539693934 MV
void deleteOldPath(string& manage_str)
{
	string new_path;//新的路径
	new_path = manage_str.substr(0, manage_str.find(" "));//ffff\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" ") + 1);//a\aadd.txt  1539693925 1539693934 MV
	manage_str = manage_str.substr(manage_str.find(" "));//1539693925 1539693934 MV
	manage_str = new_path + manage_str;
}

void deleteOldPath(string& manage_str, string& old_path)
{
	string new_path;//新的路径
	new_path = manage_str.substr(0, manage_str.find(" "));//ffff\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" ") + 1);//a\aadd.txt  1539693925 1539693934 MV
	old_path = manage_str.substr(0, manage_str.find(" "));//a\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" "));//1539693925 1539693934 MV
	manage_str = new_path + manage_str;
}

//svn git log (文件名) ---> 查看所有历史版本信息（包括版本号和提交日志）
//如果有文件名，则检索显示特定文件的历史提交信息
void gitLog(string parameter)
{
	if (parameter.find(" ") != -1)
	{
		string filename;
		filename = parameter.substr(parameter.find(" ") + 1);
		gitLogFile(filename);
	}
	else
		gitLogAll();
}
//查看所有历史版本信息（包括版本号和提交日志）
void gitLogAll()
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//遍历该文件夹
	vector<string> logs;//commit下的文件名
	getAllFiles(log_path, logs);//把文件名读入logs
	decltype(logs.size()) i;
	string log;
	for (i = 0; i < logs.size(); ++i)
	{
		log = getLogToString(logs[i]);
		cout << "version number is: " << logs[i].substr(logs[i].find_last_of("\\") + 1)
			 << ", log: " << log << endl;
	}
}
//读取文件第一行内容（日志）filename = .svn\commit\1539777011
string getLogToString(const string filename)
{
	ifstream infile(filename);       //读入文件
	string line;
	getline(infile, line); //按行读取

	return line;
}

//检索显示特定文件的历史提交信息
void gitLogFile(const string filename)
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//遍历该文件夹
	vector<string> logs;//commit下的文件名
	getAllFiles(log_path, logs);//把文件名读入logs
	decltype(logs.size()) i;
	string log;
	int flag = 0;//没有找到该文件
	for (i = 0; i < logs.size(); ++i)
	{
		if (filename == getFileInLog(logs[i]))
		{
			flag = 1;
			log = getLogToString(logs[i]);
			cout << "version number is: " << logs[i].substr(logs[i].find_last_of("\\") + 1)
				<< ", log: " << log << endl;
		}
	}
	if (flag == 0)
		cout << filename << " has no version info." << endl;
}
//读取版本文件中的文件名  filename = .svn\commit\1539777011
string getFileInLog(const string filename)
{
	ifstream infile(filename);       //读入文件
	string line;
	getline(infile, line); //按行读取
	getline(infile, line);
	line = line.substr(0, line.find(" "));

	return line;
}

//svn revert (版本号) (-f 文件名/目录名) ---> 恢复当前文件到历史版本中
/*如果不设置版本号和文件名，则默认将仓库托管的所有文件恢复到上一提交版本
**如果不设置版本号而设置文件名，则将特定文件恢复到上一版本
**如果设置版本号而不设置文件名，将仓库托管的所有文件恢复到特定提交版本
**如果既设置版本号又设置文件名，则将特定文件恢复到特定提交版本
*/
void revert(string parameter)
{
	if (parameter.empty() == true)
	{
		cout << parameter << "revert to recent version : ";
		revertAll();
	}
	else
	{
		if (parameter.substr(0, 2) == "-f")//只有文件名 svn revert -f info
		{
			string filename;
			filename = parameter.substr(parameter.find(" ") + 1);//info
			cout << filename << " revert to recent version " << endl;
			revertFile(filename);
		}
		else if (parameter.find("-f") != -1)//既有文件名，又有版本号 svn revert 1539938658 -f info
		{
			string filename;
			string version;
			filename = parameter.substr(parameter.find_last_of(" ") + 1);
			version = parameter.substr(0, parameter.find(" "));
			cout << filename << " revert to recent version : " << version;
			revertFileVersion(filename, version);
		}
		else if (parameter.find("-f") == -1)//只有版本号 svn revert 1539938658///////////////////?????
		{
			string version;
			version = parameter;
			cout << "revert to recent version : " << version;
		}
		else
			cout << "input error" << endl;
	}
}

//不设置版本号和文件名，则默认将仓库托管的所有文件恢复到上一提交版本
void revertAll()
{
	string recent_version;//最近的版本号
	recent_version = getLastVersion();//获得最近的版本号
	if (recent_version == "f")        //无法获得版本号，直接返回
		return;
	cout << recent_version << endl;

	vector<string> version_content;
	version_content = getVersionContent(recent_version);//获得版本号文件中的内容
	//恢复文件，（删除工作目录下文件），拷贝newVerison到工作目录，更改标签为C
	string filename;
	filename = subFilename(version_content[1]);///content[1]是MV？？？？？？

	revertA(filename, recent_version);
	
}

//设置文件名，则将特定文件恢复到上一版本
void revertFile(const string file)
{
	vector<string> versions;
	versions = getVersions();
	sort(versions.begin(), versions.end(), cmp);//从小到大

	string file_recent_version;//文件的版本号
	file_recent_version = getLastVersion(versions, file);//找到文件的版本号
	if (file_recent_version == "f")
	{
		cout << file << " not in  revert " << endl;
		return;
	}
	cout << file << " version number is : " << file_recent_version << endl;
	revertA(file, file_recent_version);

}
bool cmp(string a, string b)
{
	return a > b;
}
//设置版本号，将仓库托管的所有文件恢复到特定提交版本
//直接将该版本号中的文件revert
void revertVersion(const string version)
{
	vector<string> version_content;//version该版本中的内容
	version_content = getVersionContent(version);
	if (version_content.size() == 0)
	{
		cout << "This version " << version << " no exist" << endl;
		return;
	}
	string filename;
	//if (version_content.size() == 3)//第三行肯定是C，第二行是MV
	//	filename = subFilename(version_content[2]);
	//if (version_content.size() == 2)//第二行肯定是D
	//	filename = subFilename(version_content[1]);
	filename = subFilename(version_content[1]);///content[1]是MV？？？？？？

	revertA(filename, version);

}
//既设置版本号又设置文件名，则将特定文件恢复到特定提交版本
void revertFileVersion(const string file, const string version)
{
	if (cmpFileInVersion)
		revertA(file, version);
}

//获得commit下所有版本文件名
vector<string> getVersions()
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//遍历该文件夹
	vector<string> logs;//commit下的文件名
	getAllFiles(log_path, logs);//把文件名读入logs  .svn\.commit\12231223
	for (int i = 0; i < logs.size(); ++i)
	{
		logs[i] = logs[i].substr(logs[i].find_last_of("\\") + 1);
	}
	return logs;
}
//获得最近的版本号
string getLastVersion()
{
	vector<string> logs;//commit下的版本文件名
	logs = getVersions();
	
	if (logs.size() == 0)
	{
		cout << "no log" << endl;
		return "f";
	}
	int max;//logs的最大值，上一个版本
	int temp;
	//cout << logs[0]; //输出版本号 string
	max = atoi(logs[0].c_str());
	for (int i = 0; i < logs.size(); i++)
	{
		temp = atoi(logs[i].c_str());
		if (max < temp)
			max = temp;
	}
	return to_string(max);
}
//返回版本号，文件上一次提交的版本。 versions 所有版本文件名，filename 要在版本文件中找到该文件
string getLastVersion(vector<string> versions, string filename)
{
	string version = ".svn\\commit\\";
	vector<string> contents;
	int i = 0;
	for (; i < versions.size(); ++i)
	{
		contents = getVersionContent(versions[i]);//版本号日志的内容
		if (contents.size() == 0)
			continue;
		if (getLabel(contents[1]) != "MV")
		{
			if (subFilename(contents[1]) == filename)
				return versions[i];
		}
		else if (getLabel(contents[1]) == "MV")
		{
			if (subFilename(contents[2]) == filename)
				return versions[i];
		}
	}
	if (i == versions.size())
		return "f";
}


//获得版本号文件中的内容 version 版本号，如果version不存在，返回的vector.size()=0
vector<string> getVersionContent(string version)
{
	version = ".svn\\commit\\" + version;
	ifstream infile(version);       //读入文件
	//if (!infile)
	//	cout << "This version " << version << " no exist" << endl;
	vector<string> lines;           //版本号内容，三行
	string line;
	while (getline(infile, line))
	{
		lines.push_back(line);
	}
	return lines;
}

//恢复文件filename（版本中的）
void revertA(string filename, string version_number)
{
	string label;
	label = getLabelInManage(filename);//在manage_file中找到文件filename，返回label
	//没有找到文件，返回label="f"
	if (label == "f")
		return;
	string new_version_file;
	new_version_file = ".svn\\copyfile\\" + version_number + "\\" + filename;

	int flag = 0;
	//标签为C 且修改过
	if (label == "C" && diff(filename))
	{
		copyFile(new_version_file, filename);
		flag = 1;
	}
	//恢复修改或删除的文件，文件没移动过，
	if (label == "M" || label=="D")
	{
		//remove(filename.c_str());//移除工作目录下文件
		copyFile(new_version_file, filename);
		revertManageFile(filename, label);
		flag = 1;
	} 
	//删除工作目录下被移动的文件，拷贝newVersion下的文件到工作目录，manage_file 保留version目录 
	else if (label == "MV")
	{
		string working_path_name = getWorkingPath(filename); //工作目录下的路径
		//filename是版本下的目录（未被移动过）
		remove(working_path_name.c_str());//工作目录下的
		copyFile(new_version_file, filename);
		revertManageFile(working_path_name, label);
		flag = 1;
	}
	if (flag == 0)
		cout << filename << "no need to revert" << endl;
}
//给文件名，找到manage_file中的label，manage_file找不到该文件（delete了）
string getLabelInManage(string filename)
{
	string manage_path = ".svn\\manage_file";
	vector<string> content;
	string new_version_path;
	readFileIntoVector(manage_path, content);
	int i;
	for (i = 0; i < content.size(); ++i)
	{
		if (subFilename(content[i]) == filename)
			return getLabel(content[i]);
		else if (getNewVersionPath(content[i]) == filename)//newVersion path
			return getLabel(content[i]);
	}
	if (i == content.size())
	{
		cout << "can not revert " << filename << " in repo(manage_file)" << endl;
		return "f";
	}
}
//通过Version中的文件目录filename，找到 工作目录下的 路径
string getWorkingPath(string filename)
{
	string manage_path = ".svn\\manage_file";
	vector<string> content;
	string new_version_path;
	readFileIntoVector(manage_path, content);
	int i;
	for (i = 0; i < content.size(); ++i)
	{
		if (getNewVersionPath(content[i]) == filename)//newVersion path
			return subFilename(content[i]);
	}
	if (i == content.size())
	{
		cout << "can not revert " << filename << " , can not find it in working path." << endl;
		return "f";
	}
}
//MV 返回newVersion 下的目录
string getNewVersionPath(string line)
{
	line = line.substr(line.find(" ") + 1);
	line = line.substr(line.find(" ") + 1);
	line = line.substr(0, line.find(" "));
	return line;
}

//根据commit version里的文件名和  恢复manage_file到上一版本
//filename 要恢复的文件名，
void revertManageFile(string filename, string label)
{
	string manage_path = ".svn\\manage_file";

	vector<string> content;
	readFileIntoVector(manage_path, content);//读manage_file文件内容到content
	//遍历content中有无filename，有 则根据label值对它进行修改
	for (int i = 0; i < content.size(); ++i)
	{
		//cout << subFilename(content[i]) << endl;
		if (subFilename(content[i]) == filename)
		{
			if (label == "M" || label == "D")
			{
				modifyLabelInFile(manage_path, i + 1, "C");//直接在文件里改好了
			}
			else if (label == "MV")
			{
				
				//删除前两个路径，留下newVersion中的路径
				deleteOldPath(content[i]);
				content[i] = content[i].substr(content[i].find(" ") + 1);//删除新路径

				writeVectorIntoFile(manage_path, content);//将content写入文件

				modifyLabelInFile(manage_path, i + 1, "C");//直接在文件里改好了标签C
			}
			else
				cout << "can not revert" << filename << endl;
			return;
		}
	}
}

//该版本信息中是否包含该文件
bool cmpFileInVersion(string version, string filename)
{
	version = ".svn\\commit\\" + version;
	vector<string> contents;
	int i = 0;
	
	contents = getVersionContent(version);//版本号日志的内容
	if (getLabel(contents[1]) != "MV")
	{
		if (subFilename(contents[1]) == filename)
			return true;
	}
	else if (getLabel(contents[1]) == "MV")
	{
		if (subFilename(contents[2]) == filename)
			return true;
	}
	
	cout << "this version do not have this file" << endl;
	return false;
}