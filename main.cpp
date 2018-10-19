#include<iostream>
#include<string>
#include<io.h>     //_access
#include<direct.h> //_mkdir, getcwd
#include<vector>
#include<fstream>  //ofstream
#include <sstream> //ostringstream
#include<time.h>   //time(0)
#include<algorithm>//min()
#include <cstring> //cout����ȷ
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

//Ŀǰֻ����main.cpp�ļ�Ŀ¼�´����ֿ�.svn
int main()
{
	string command;//��������ָ��
	getline(cin, command);

	if (command.find("svn") != -1)
	{
		string function;//ָ���
		//"svn "������ַ�����
		function = command.substr(command.find("svn ",0) + 4);//��0��ʼ���ң�����svn ���ֵ�λ�ã�+4��ʾ��svn ��֮���һλ
		string func;// create...
		string parameter; // ������Ŀ¼���ļ����ȣ�
		if (function.find(" ") != -1) //function�к���" "
		{
			func = function.substr(0, function.find_first_of(" "));//commit     aadd.txt -m 'x'
			string temp;
			temp = function.substr(function.find_first_of(" "));//    aadd.txt -m 'x'(���˿ո�)
			parameter = temp.substr(temp.find_first_not_of(" "));//aadd.txt -m 'x'
		}
		else
		{
			func = function;//status, diff...
		}

		//svn create [Ŀ¼��]
		if (func == "create")
		{
			create(parameter);
		}
		//svn status
		else if (func == "status")
		{
			status();
		}
		//svn add [�ļ���/Ŀ¼��]
		//svn add ffff\f1\ff1.txt
		else if (func == "add")
		{
			add(parameter);
		}
		//svn commit [�ļ���/Ŀ¼��] (-m '�ύ��־')
		else if (func == "commit")
		{
			commit(parameter);
		}
		//svn delete [�ļ���] (--force)
		else if (func == "delete")
		{
			del(parameter);
		}
		//svn mv [�ļ���/Ŀ¼��] [Ŀ��Ŀ¼]
		else if (func == "mv")
		{
			move(parameter);
		}
		//svn git log (�ļ���)
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
	}
	else
	{
		cout << "command error" << endl;
	}

	system("pause");
	return 0;
}

//svn create [Ŀ¼��]     svn create test
//��ʼ��һ���汾�ֿ⣬�ڵ�ǰĿ¼�´���һ��Ŀ¼��Ŀ¼�ڳ�ʼ��һ��.svn�ļ��У���.svn�ļ�����Ӧ�ü�¼Щ������
//���ڴ洢�ֿ�汾���ݣ������ǰĿ¼����һ���汾�ֿ��£�����ʧ��
void create(string repo_name)
{
	string svn_path;
	if (repo_name.empty() == 1)//û������������ֿ�����
	{
		//�жϵ�ǰĿ¼���Ƿ� ����.svn�ļ��У�����һ���汾�ֿ��£�
		if (_access(".svn", 0) == -1)//��ǰĿ¼�²�����.svn
		{
			svn_path = ".svn";
			_mkdir(".svn");//��ǰĿ¼�´���.svn�ļ�

			//��Ҫ����һϵ���ļ�
			create_init(svn_path);
		}
		else
			cout << "Repository exists, create fails" << endl;
	}
	else //�ֿ�����Ϊ��
	{
		_mkdir(repo_name.c_str());//�����ֿ�repo_name
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

//����.svn�µ�Ŀ¼�ͼ�¼�ļ���svn_path = ".svn";����svn_path = repo_name + "//.svn";
void create_init(string svn_path)
{
	cout << "Create an empty Svn repository" << endl;
	//�����ļ���commit���汾��Ϣ��,copyfile�����ļ�
	_mkdir((svn_path + "\\commit").c_str());
	_mkdir((svn_path + "\\copyfile").c_str());
	_mkdir((svn_path + "\\copyfile\\newVersion").c_str());

	//�����ļ�manage_file���ܹ����ļ�����Ϣ��
	ofstream manage;
	manage.open(svn_path + "\\manage_file", ios::out);//"manage_file"���ļ�������д�뷽ʽ���ļ���˫б��
	manage.close();
}

//��ȡ��ǰ����Ŀ¼
void workingDir(string &filePath)
{
	char   buffer[WORK_PATH];
	getcwd(buffer, WORK_PATH);//getcwd()�Ὣ��ǰ����Ŀ¼�ľ���·�����Ƶ�����buffer��ָ���ڴ�ռ���,����maxlenΪbuffer�Ŀռ��С��
	filePath.assign(buffer);//��ǰ�ļ��е�·����D:\cpp\svn\in_this_dir\in_this_dir
}

//svn status �鿴��ǰ�ֿ��ļ������з����仯���ļ�
//��+ b.cpp����ʾ�������ļ�
void status()
{
	string working_path;
	workingDir(working_path);

	//��ȡ����Ŀ¼�������ļ�
	vector<string> files;//����Ŀ¼�������ļ������·��
	getAllFilesInfo(working_path, files);//files�ǵ�ǰ�ļ�
	
	//��manage_file�е��ļ����бȽϣ���manage_file�в������޸��ļ���
	vector<string> manage_str; //manage_file�ļ��е�����
	readFileIntoVector(".svn\\manage_file", manage_str);
	//vector<string> managed_files;
	//vector<string> unmanaged_files;
	string filename;//����Ŀ¼�������ļ�
	string manage_file;
	for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i)
	{
		filename = subFilename(*i);
		for (int j = 0; j < manage_str.size(); ++j)
		{
			manage_file = subFilename(manage_str[j]);
			if (filename == manage_file)//filename���ļ�manage_file���ҵ���˵���ѱ�����
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
					//manage_file(filename)���ĺ���µ�ַ����manage_str[j]�ж���ԭʼ��ַ
					string old_path = manage_str[j].substr(manage_str[j].find(" ") + 1);
					old_path = old_path.substr(0, old_path.find(" "));
					cout << "MV " << old_path << " .\\" << old_path << " -> .\\" << filename << endl;
					
				}
				if (label == "C")
				{
					//����Ƿ��޸�
					if (diff(filename))
					{
						modifyLabelInFile(".svn\\manage_file", j + 1, "M");
						//��ͬ���޸�label
						cout << "M " << filename << endl;
					}
					else
						cout << "C " << filename << endl;
				}
				break;
			}
			if (j == manage_str.size() - 1)
				cout << "? " << filename << endl;
		}
		
	}
}

//���str��label
const string getLabel(const string str)
{
	return str.substr(str.find_last_of(" ") + 1);
}

//getFiles(filePath, files);
//��ȡ��ǰ�ļ����������ļ����ļ���, pathΪ��ǰ�ļ��е�ȫ·��working_path
//�����files���·��
void getAllFiles(string path, vector<string>& files)
{
	//�ļ���� 
	long  hFile = 0;
	//�ļ���Ϣ 
	struct _finddata_t fileinfo;
	string p;
	string working_dir;
	workingDir(working_dir);
	working_dir = working_dir + "\\";
	int len = working_dir.size();
	string temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//������ҳɹ��Ļ���������һ��long�͵�Ψһ�Ĳ����õľ��������һ��Ψһ��ţ������򷵻�-1
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//atrrib���ļ����ԵĴ洢λ�á�_A_SUBDIR���ļ��У�
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".svn") != 0)
				{
					temp = p.assign(path).append("\\").append(fileinfo.name);
					temp = temp.substr(temp.find(working_dir, 0) + len);//ȥ������·��
					//string create_time = to_string(fileinfo.time_create);//��Ӵ���ʱ��
					files.push_back(temp);
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				temp = p.assign(path).append("\\").append(fileinfo.name);
				temp = temp.substr(temp.find(working_dir, 0) + len);
				//string create_time = to_string(fileinfo.time_create);//��Ӵ���ʱ��
				files.push_back(temp);
			}
		} while (_findnext(hFile, &fileinfo) == 0);//���ɹ�����0�����򷵻�-1��
		_findclose(hFile);
	}
}

//getFiles(filePath, files);
//��ȡ��ǰ�ļ����������ļ����ļ��У������������޸�ʱ�䣩, pathΪ��ǰ�ļ��е�ȫ·��
void getAllFilesInfo(string path, vector<string>& files)
{
	//�ļ���� 
	long  hFile = 0;
	//�ļ���Ϣ 
	struct _finddata_t fileinfo;
	string p;
	string working_dir;
	workingDir(working_dir);
	working_dir = working_dir + "\\";
	int len = working_dir.size();
	string temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//������ҳɹ��Ļ���������һ��long�͵�Ψһ�Ĳ����õľ��������һ��Ψһ��ţ������򷵻�-1
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//atrrib���ļ����ԵĴ洢λ�á�_A_SUBDIR���ļ��У�
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".svn") != 0)
				{
					temp = p.assign(path).append("\\").append(fileinfo.name);
					temp = temp.substr(temp.find(working_dir, 0) + len);//ȥ������·��
					string create_time = to_string(fileinfo.time_create);//��Ӵ���ʱ��
					string modified_time = to_string(fileinfo.time_write);//����޸�ʱ��
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
		} while (_findnext(hFile, &fileinfo) == 0);//���ɹ�����0�����򷵻�-1��
		_findclose(hFile);
	}
}

//���ļ���ȡ��Ϣ��string��, ".svn//manage_file"
string readFileIntoString(const string filename)
{
	ifstream infile(filename);
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf&&infile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}

//���ļ���ȡ��Ϣ��vector<string>��, ".svn//manage_file"
void readFileIntoVector(const string filename, vector<string>& content)
{
	ifstream infile(filename);       //�����ļ�
	string line;
	while (getline(infile, line))   //���ж�ȡ
	{
		content.push_back(line);
	}
}

//���ļ������������·����ȡ��������ȥ��ʱ���������Ϣ
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

//svn add [�ļ���/Ŀ¼��] 
//��һ��δ�ܹ�����ļ���ӵ������룬
//����ѱ����������κ�Ч����ע����git add���֡���git add ���޸ĺ�Ϳ���add��
void add(string file_name)
{
	if (file_name.empty() == false)
	{
		string working_path;//��ǰ����Ŀ¼
		workingDir(working_path);
		vector<string> files;//����Ŀ¼�������ļ������·��
		getAllFilesInfo(working_path, files);//Ҫ�Ұ����޸�ʱ�����Ϣ
		string temp_file;
		int fileNum = 0;

		//����files����Ҫadd���ļ���file_name���Ƿ��ѱ��йܣ���manage_file����
		for (vector<string>::iterator i = files.begin(), end = files.end(); i != end; ++i, ++fileNum)
		{
			//���ж�Ҫadd���ļ���file_name���Ƿ��ڹ���Ŀ¼���ļ��У�files��
			temp_file = subFilename(*i);//����Ŀ¼���ļ���
			if (temp_file == file_name)
			{
				if (file_name == temp_file)//����������ҵ�Ҫ��ӵ��ļ�/�ļ��У������ļ����Ϸ��������жϸ��ļ��Ƿ��ѱ��йܡ��޸�
				{
					vector<string> manage_vec;//����manage_file��vector
					readFileIntoVector(".svn\\manage_file", manage_vec);
					//����manage_vec���ҵ�file_name�Ƿ��й�
					for (vector<string>::iterator j = manage_vec.begin(), e = manage_vec.end(); j != e; ++j)
					{
						if (file_name == subFilename(*j))//�ѱ��йܣ��޷���add
						{
							cout << file_name << " is managed" << endl;
							return;
						}
					}
					//����manage_file�л�manage_fileΪ�գ�������йܣ�manage_file��
					ofstream manage;
					manage.open(".svn\\manage_file", ios::app);//manage_file�ļ�׷���������
					manage << (*i).append(" +") << endl;       //����ļ���Ϣ����ĩβ����+���ϱ��
					manage.close();
					cout << "add " << subFilename(*i) << " to repo" << endl;
					return; //��������
				}
			}
		}
		if (fileNum == files.size())//����������Ŀ¼�µ��ļ�
		{
			cout << file_name << " is not in directory" << endl;
			return;
		}
	}
	else
		cout << "add ???" << endl;
}


//ÿһ��commit�������һ���µİ汾
//svn commit [�ļ���/Ŀ¼��] (-m '�ύ��־')     ��git������
//�ύ�޸ģ���¼�����ύ�޸ĵ��ļ������޸����ݣ�ͬʱ������־��commit�ļ�����
//����ύ�ɹ�������һ�������汾�ţ����û���ļ������仯���ύʧ�ܣ���ʾ��no modification��
//Ϊ�����պ�ָ�������.svn ����һ���°汾�ļ��ı��ݣ�copyfile�ļ�����
///////////////////////////////////////////////////     MV �ָ�newVersion�е�·��   D ɾ��
void commit(string parameter)
{
	string filename;//�ļ���/Ŀ¼��
	filename = parameter.substr(0, parameter.find_first_of(" ", 0));
	if (filename == "-m")//û�������ļ�/Ŀ¼��
	{
		cout << "please inter file or directory name��"<<endl
				<< "svn commit [�ļ���/Ŀ¼��] (-m '�ύ��־')" << endl;
		return;
	}

	//�����ļ�����1���Ƿ����йܣ���manage_file�У���2���Ƿ�����'C'
	vector<string> file; //��ȡmanage_file�ļ���ÿ��
	readFileIntoVector(".svn\\manage_file", file);


	int flag = 0;//����ļ��Ƿ����йܣ�manage_file�У�
	int line = 1;
	vector<string> commit_before;   // ��ӿ���commit���ļ���commitǰ���ļ���Ϣ �����Ը���string��ֻ����һ����
	vector<string> commit_after;    // commit����ļ���Ϣ

	//��manage_file�ļ��е�ÿ�н��б���������Ƿ����commit�����+ M D MV����
	//���������commit_file�����޸�manage_file�ļ��еı�ǩ
	string old_label;//ԭ�еı��
	for (vector<string>::iterator i = file.begin(), end = file.end(); i != end; ++i, ++line)
	{
		old_label = getLabel(*i);
		if (filename==subFilename(*i))  // ����ļ������ڲֿ���
		{
			flag = 1;
			if (old_label == "C")
			{
				//֮ǰcommit�� ����Ƿ��޸ģ�Դ�ļ���newVersion�Ա�
				if (diff(filename))
				{
					modifyLabelInFile(".svn\\manage_file", line, "M");
					old_label = "M";
				}

			}
			if (old_label == "D")
			{
				deleteContentInFile(".svn\\manage_file", line);//�����D����ɾ��manage_file�и��ļ�
				commit_before.push_back(*i);
				//�����Ҫɾ�����ļ�����û��commit֮�����Ϣ
			}
			if (old_label == "+" || old_label == "M" || old_label == "MV")
			{
				string temp;
				string old_path;//MV ��ԭ����path�������ļ�����
				temp = (*i);
				//commit֮�� MV�ļ� Ҫ��newVerion�и��ģ�ɾ��ԭ���ģ�����µ� ·���ļ������޸�manage_fileΪcommit֮���
				if (old_label == "MV")
				{
					deleteOldPath(temp, old_path);//temp���µ��ļ���Ϣ�����old_path����һ�Σ��ƶ�ǰ����·��
					deleteOldPath(temp, old_path);//                   ���old_path��newVersion��·��
					//ɾ��newVersion��old path
					old_path = ".svn\\copyfile\\newVersion\\" + old_path;
					remove(old_path.c_str());
					//��manage_file Ϊtemp
					modifiedContentInFile(".svn\\manage_file", line, temp);//�޸�ĳ������
				}

				modifyLabelInFile(".svn\\manage_file", line, "C");

				commit_before.push_back(*i);      //��������������ļ�+ M D MV������ԭʼ�ģ��浽commit�汾�ļ��У�
				temp = temp.substr(0, temp.find_last_of(" "));//ȥ����ǩ
				temp = temp.append(" C");
				commit_after.push_back(temp);
			}
		}
	}
	if (commit_before.empty() == false) //�ļ����޸ĵ��ļ�+ M D MV����Ϊ�գ��ύ�޸�
	{
		string log = "";//�ύ��־
		if (parameter.find("-m ") != -1)// parameter.find(" -m ")  4294967295�൱��-1
		{
			log = parameter.substr(parameter.find(" -m ", 0) + 5);
			int len = log.size() - 1;
			log = log.substr(0, len);
		}

		//���ص����汾�ţ�����ǰʱ����Ϊ�����汾�ţ�
		time_t currentTime = time(0);
		string versionNum = to_string(currentTime);
		//������¼�ļ����԰汾��������Ϊ�ļ������� ��commitĿ¼��
		string commit_path = ".svn\\commit\\";
		ofstream version;
		version.open(commit_path + versionNum, ios::out);
		version << log << endl;//version�ļ�д���ύ��־log
		for (vector<string>::iterator i = commit_before.begin(), end = commit_before.end(); i != end; ++i)
		{
			version << (*i) << endl;//д������ӵ��ļ��ʹ����޸�ʱ��
			if (old_label != "D")
				version << commit_after[0] << endl;
		}
		version.close();

		//����
		if (old_label == "D")
			backupD(versionNum, commit_before);//ɾ��newVersion�µ��ļ�
		else
			backup(versionNum, commit_before);

		cout << "commit success, version number is : " << versionNum << endl;
	}
	else    //commit_file is empty
	{
		if (flag == 1)//�ļ��ڲֿ⣨��û���޸ģ�
			cout << filename << " no modification" << endl;
		else
			cout << filename << " did not add to repo" << endl;
	}
}

//���ݹ���Ŀ¼�µ��ļ��� copyfile (newVersion��versionNum)
void backup(const string versionNum, vector<string> commit_file)
{
	//����
	string copy_path = ".svn\\copyfile\\";
	_mkdir((copy_path + versionNum).c_str());//��copyfile�´����ļ��У��԰汾��Ϊ����

	string source_file;//����Ŀ¼�µ��ļ�
	string copy_file;
	for (size_t i = 0; i < commit_file.size(); i++)
	{
		source_file = subFilename(commit_file[i]);
		
		string new_version_path = ".svn\\copyfile\\newVersion\\";
		string version_num_path = ".svn\\copyfile\\" + versionNum + "\\";

		//Ҫ���ݵ��ļ��к���\����Ŀ¼����Ҫ�ȴ�����Ŀ¼
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

		copyFile(source_file, version_num_path  + copy_file);//���ļ�д��汾���ļ���
		copyFile(source_file, new_version_path  + copy_file); //���ļ�д���°汾�ļ��У������ͬ�ļ������������ļ���ô�죿������
	}
}
//����ɾ�����ļ���versionNum)��ɾ��newVersion�µ��ļ�
void backupD(const string versionNum, vector<string> commit_file)
{
	string copy_path = ".svn\\copyfile\\";
	_mkdir((copy_path + versionNum).c_str());//��copyfile�´����ļ��У��԰汾��Ϊ����

	string source_file;//����Ŀ¼�µ��ļ�
	string copy_file;
	for (size_t i = 0; i < commit_file.size(); i++)
	{
		source_file = subFilename(commit_file[i]);

		string version_num_path = ".svn\\copyfile\\" + versionNum + "\\";//backup
		string new_version_path = ".svn\\copyfile\\newVersion\\" + source_file;//delete

		//Ҫ���ݵ��ļ��к���\����Ŀ¼����Ҫ�ȴ�����Ŀ¼
		if (source_file.find("\\") != -1)
		{
			version_num_path = version_num_path + source_file.substr(0, source_file.find_last_of("\\")) + "\\";
			mkDir(version_num_path);
		}
		copy_file = source_file.substr(source_file.find_last_of("\\") + 1);
		copyFile(source_file, version_num_path + copy_file);//���ļ�д��汾���ļ���
		remove(new_version_path.c_str());
	}
}


//�����ļ���copyFile("f:/info", "f:/coper");
void copyFile(string pszFileCopee, string pszFileCoper)
{
	fstream fsCopee(pszFileCopee, ios::binary | ios::in);
	fstream fsCoper(pszFileCoper, ios::binary | ios::out);
	fsCoper << fsCopee.rdbuf();
}

//ʶ�������ı��Ĳ�ͬ�У�������һ���µ���ʱ�ı� temp
//�ı���Ӧλ����ʾ������Դ�ı���ͬ�벻֮ͬ������֮ͬ�����Ǵ������ĸ�Դ�ļ�
//	string f1 = "info";string f2 = "copyer";���������ļ������·�� ffff\f1.txt
int diff2(const string f1, const string f2)
{
	vector<string> v1, v2;
	readFileIntoVector(f1, v1);
	readFileIntoVector(f2, v2);

	int flag = 0;//��ʾ�����ļ���ͬ
	ofstream temp; //д���ļ�
	temp.open(".svn\\temp", ios::out);
	decltype(v1.size()) i = 0;
	for (; i < min(v1.size(), v2.size()); ++i)
	{
		if (v1[i] != v2[i])
		{
			//cout << "different line: " << i + 1 << endl;//��֮ͬ�����к�
			//�ı���Ӧλ����ʾ������Դ�ı���֮ͬ��
			temp << "diff:" << f1 << ":" << v1[i] << "|||" << f2 << ":" << v2[i] << endl;
			flag = 1;
		}
		else
			temp << v1[i] << endl;
	}
	if (v1.size() > v2.size())//f1�ı����ȴ���f2
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
	return flag;//����1��ʾ�����ļ���ͬ
}

//�鿴�����йܵ�ĳ���ļ��Ƿ��޸ģ������ļ��ڹ���Ŀ¼�µ����·��
//����1��ʾ�����ļ���ͬ
int diff(string file)
{
	if (searchInManage(file))//���й����ҵ��ļ�
	{
		string f1, f2;
		f1 = file;
		f2 = ".svn\\copyfile\\newVersion\\" + file;
		int flag = 0;//0��ʾ�ļ���ͬ
		flag = diff2(f1, f2);
		return flag;////����1��ʾ�����ļ���ͬ
	}
	else
	{
		cout << "this file did not been managed" << endl;
		return 0;
	}
}

//��manage_file���ҵ�file��������
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

//�����°汾�ļ��У��ҵ�file �����ļ�·��
void searchInNewVersion(const string file)
{
	string new_version_path = "";
	string working_path;
	workingDir(working_path);
	new_version_path = working_path + "\\.svn\\copyfile\\newVersion";
	vector<string> files;//���°汾�µ��ļ�
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

//�������Ŀ¼  path="test\\test1\\test2"
void mkDir(string path)
{
	int m = 0;
	string temp_path;
	while (m >= 0)
	{
		m = path.find("\\");//û���ҵ�����-1���ҵ����ص�һ���ַ�������
		temp_path += path.substr(0, m) + "\\";
		if (_access(temp_path.c_str(), 0) == -1)
			_mkdir(temp_path.c_str());
		if (m != -1)
			path = path.substr(m + 1, path.size());// "\\"+1,   "//"+2,  \\ת���ַ�
	}
}

//��vector������д���ļ�
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

//�޸�manage_fileÿ��֮��ı�ǩ��filenameҪ���ĵ��ļ� .svn\manage_file��lineNumҪ�ĵ��ļ��е��кţ�label�ĺ�ı�ǩ
void modifyLabelInFile(string filename, int lineNum, string label)
{
	vector<string> content;
	readFileIntoVector(filename, content);//���ļ�����vector��
	int index = lineNum - 1;//Ҫ�޸ĵ���
	string modify_line = content[index];//Ҫ�޸ĵ�����
	modify_line = modify_line.substr(0, modify_line.find_last_of(" "));//ɾ��ԭ�б�ǩ
	modify_line = modify_line.append(" ").append(label);//��ӱ�ǩ
	content[index] = modify_line;

	//vectorд���ļ�
	writeVectorIntoFile(filename, content);
}


//svn delete [�ļ���] (--force)����һ���ܹ�����ļ��ɳ�����״̬����ɾ�������ļ���
//--->>��manage_file�и��ļ����ΪD������ɾ��newVersion�е���Ϣ��commit֮���ɾ����
//���δ�����������κ�Ч����ͨ��--forceɾ��ʵ���ļ�
void del(string parameter)
{
	string file;
	bool force = false;//Ĭ�ϲ�ɾ��ʵ���ļ�
	if (parameter.find(" --force") != -1)
	{
		force = true;
		file = parameter.substr(0, parameter.find(" "));//��ȡ�ļ���
	}
	else
		file = parameter;
	//�ж��ļ��Ƿ񱻹��� manage_file
	vector<string> manage_file;
	readFileIntoVector(".svn\\manage_file", manage_file);
	for (int i = 0; i < manage_file.size(); ++i)
	{
		if (subFilename(manage_file[i]) == file)//�ҵ�manage_file�е��ļ�
		{
			string label = getLabel(manage_file[i]);
			if (label != "C")//���ΪC���ļ����Ա�delete
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
	//δ������
	cout << "delete fail, " << file << " did not been managed" << endl;
}

//ɾ���ļ��е�ĳһ��
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
//���ļ����޸�һ��
void modifiedContentInFile(string filename, int lineNum, string contentLine)
{
	vector<string> content;
	readFileIntoVector(filename, content);//�����ļ���content
	content[lineNum - 1] = contentLine;//�޸�content
	writeVectorIntoFile(filename, content);//��contentд���ļ�
}

//svn mv [�ļ���/Ŀ¼��] [Ŀ��Ŀ¼]
//�ƶ��ļ����У���ע����²ֿ��ļ�λ�ü�¼��manage_file��
//mv ���˶�Σ�old_path һֱ�䣬���ֿ�newVersion��pathû�䣨Ҫ��¼��
//ffff\aadd.txt a\aadd.txt a\aadd.txt 1539693925 1539693934 MV
//���ĺ��Ŀ¼  ԴĿ¼     newVersionĿ¼
void move(string parameter)
{
	string source_file, goal_path ,filename;
	source_file = parameter.substr(0, parameter.find(" "));//Դ�ļ�·��+����
	goal_path = parameter.substr(parameter.find(" ") + 1);//Ŀ��Ŀ¼
	if (source_file.find('\\') != -1)//Դ�ļ�����Ŀ¼
		filename = source_file.substr(source_file.find("\\") + 1);
	else
		filename = source_file;
	

	vector<string> manage_file;
	string change_path;//���ĺ��·��
	readFileIntoVector(".svn\\manage_file", manage_file);
	
	for (int i = 0; i < manage_file.size(); ++i)
	{
		if (subFilename(manage_file[i]) == source_file)//��manage_file���ҵ�Ҫ�ƶ����ļ�
		{
			if (getLabel(manage_file[i]) == "MV" || getLabel(manage_file[i]) == "C")//ֻ�б��Ϊ MV �� C���ļ����ܱ�mv
			{
				change_path = goal_path + "\\" + filename;
				//�ƶ��ļ�
				copyFile(source_file, change_path);
				remove(source_file.c_str());

				//���²ֿ��¼ path ���MV
				if (getLabel(manage_file[i]) == "C") //����newVersion�µ�·��
				{
					manage_file[i] = source_file + " " + manage_file[i];
				}
				if (getLabel(manage_file[i]) == "MV")
				{
					deleteOldPath(manage_file[i]);//���ԭ���ļ�����MV Ҫɾ���м��·��
				}
				if (change_path.substr(0,2)==".\\")//�����Ŀ¼�������ǰĿ¼�£�  .\��ȥ����
				{
					manage_file[i] = change_path.substr(change_path.find(".\\") + 2) + " " + manage_file[i]; 
				}
				else
					manage_file[i] = change_path + " " + manage_file[i]; //���º��·���ļ�+ԭ�е�·���ļ�   
				manage_file[i] = manage_file[i].substr(0, manage_file[i].find_last_of(" "));//ɾ�����
				manage_file[i] = manage_file[i].append(" ").append("MV");//��ӱ��
				writeVectorIntoFile(".svn\\manage_file", manage_file);  //д���ļ�
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
	string new_path;//�µ�·��
	new_path = manage_str.substr(0, manage_str.find(" "));//ffff\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" ") + 1);//a\aadd.txt  1539693925 1539693934 MV
	manage_str = manage_str.substr(manage_str.find(" "));//1539693925 1539693934 MV
	manage_str = new_path + manage_str;
}

void deleteOldPath(string& manage_str, string& old_path)
{
	string new_path;//�µ�·��
	new_path = manage_str.substr(0, manage_str.find(" "));//ffff\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" ") + 1);//a\aadd.txt  1539693925 1539693934 MV
	old_path = manage_str.substr(0, manage_str.find(" "));//a\aadd.txt
	manage_str = manage_str.substr(manage_str.find(" "));//1539693925 1539693934 MV
	manage_str = new_path + manage_str;
}

//svn git log (�ļ���) ---> �鿴������ʷ�汾��Ϣ�������汾�ź��ύ��־��
//������ļ������������ʾ�ض��ļ�����ʷ�ύ��Ϣ
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
//�鿴������ʷ�汾��Ϣ�������汾�ź��ύ��־��
void gitLogAll()
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//�������ļ���
	vector<string> logs;//commit�µ��ļ���
	getAllFiles(log_path, logs);//���ļ�������logs
	decltype(logs.size()) i;
	string log;
	for (i = 0; i < logs.size(); ++i)
	{
		log = getLogToString(logs[i]);
		cout << "version number is: " << logs[i].substr(logs[i].find_last_of("\\") + 1)
			 << " log: " << log << endl;
	}
}
//��ȡ�ļ���һ�����ݣ���־��filename = .svn\commit\1539777011
string getLogToString(const string filename)
{
	ifstream infile(filename);       //�����ļ�
	string line;
	getline(infile, line); //���ж�ȡ

	return line;
}

//������ʾ�ض��ļ�����ʷ�ύ��Ϣ
void gitLogFile(const string filename)
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//�������ļ���
	vector<string> logs;//commit�µ��ļ���
	getAllFiles(log_path, logs);//���ļ�������logs
	decltype(logs.size()) i;
	string log;
	int flag = 0;//û���ҵ����ļ�
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
//��ȡ�汾�ļ��е��ļ���  filename = .svn\commit\1539777011
string getFileInLog(const string filename)
{
	ifstream infile(filename);       //�����ļ�
	string line;
	getline(infile, line); //���ж�ȡ
	getline(infile, line);
	line = line.substr(0, line.find(" "));

	return line;
}

//svn revert (�汾��) (-f �ļ���/Ŀ¼��) ---> �ָ���ǰ�ļ�����ʷ�汾��
/*��������ð汾�ź��ļ�������Ĭ�Ͻ��ֿ��йܵ������ļ��ָ�����һ�ύ�汾
**��������ð汾�Ŷ������ļ��������ض��ļ��ָ�����һ�汾
**������ð汾�Ŷ��������ļ��������ֿ��йܵ������ļ��ָ����ض��ύ�汾
**��������ð汾���������ļ��������ض��ļ��ָ����ض��ύ�汾
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
		if (parameter.substr(0, 2) == "-f")//ֻ���ļ��� svn revert -f info
		{
			string filename;
			filename = parameter.substr(parameter.find(" ") + 1);//info
			cout << filename << " revert to recent version " << endl;
			revertFile(filename);
		}
		else if (parameter.find("-f") != -1)//�����ļ��������а汾�� svn revert 1539938658 -f info
		{
			string filename;
			string version;
			filename = parameter.substr(parameter.find_last_of(" ") + 1);
			version = parameter.substr(0, parameter.find(" "));
			cout << filename << " revert to recent version : " << version;
			revertFileVersion(filename, version);
		}
		else if (parameter.find("-f") == -1)//ֻ�а汾�� svn revert 1539938658///////////////////?????
		{
			string version;
			version = parameter;
			cout << "revert to recent version : " << version;
		}
		else
			cout << "input error" << endl;
	}
}

//�����ð汾�ź��ļ�������Ĭ�Ͻ��ֿ��йܵ������ļ��ָ�����һ�ύ�汾
void revertAll()
{
	string recent_version;//����İ汾��
	recent_version = getLastVersion();//�������İ汾��
	if (recent_version == "f")        //�޷���ð汾�ţ�ֱ�ӷ���
		return;
	cout << recent_version << endl;

	vector<string> version_content;
	version_content = getVersionContent(recent_version);//��ð汾���ļ��е�����
	//�ָ��ļ�����ɾ������Ŀ¼���ļ���������newVerison������Ŀ¼�����ı�ǩΪC
	string filename;
	filename = subFilename(version_content[1]);

	revertA(filename, recent_version);
	
}

//�����ļ��������ض��ļ��ָ�����һ�汾
void revertFile(const string file)
{
	vector<string> versions;
	versions = getVersions();
	sort(versions.begin(), versions.end(), cmp);//��С����

	string file_recent_version;//�ļ��İ汾��
	file_recent_version = getLastVersion(versions, file);//�ҵ��ļ��İ汾��
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
//���ð汾�ţ����ֿ��йܵ������ļ��ָ����ض��ύ�汾
void revertVersion(const string version)
{

}
//�����ð汾���������ļ��������ض��ļ��ָ����ض��ύ�汾
void revertFileVersion(const string file, const string version)
{
	if (cmpFileInVersion)
		revertA(file, version);
}

//���commit�����а汾�ļ���
vector<string> getVersions()
{
	string log_path;
	workingDir(log_path);
	log_path = log_path + "\\.svn\\commit";
	//�������ļ���
	vector<string> logs;//commit�µ��ļ���
	getAllFiles(log_path, logs);//���ļ�������logs  .svn\.commit\12231223
	for (int i = 0; i < logs.size(); ++i)
	{
		logs[i] = logs[i].substr(logs[i].find_last_of("\\") + 1);
	}
	return logs;
}
//�������İ汾��
string getLastVersion()
{
	vector<string> logs;//commit�µİ汾�ļ���
	logs = getVersions();
	
	if (logs.size() == 0)
	{
		cout << "no log" << endl;
		return "f";
	}
	int max;//logs�����ֵ����һ���汾
	int temp;
	//cout << logs[0]; //����汾�� string
	max = atoi(logs[0].c_str());
	for (int i = 0; i < logs.size(); i++)
	{
		temp = atoi(logs[i].c_str());
		if (max < temp)
			max = temp;
	}
	return to_string(max);
}
//���ذ汾�ţ��ļ���һ���ύ�İ汾�� versions ���а汾�ļ�����filename Ҫ�ڰ汾�ļ����ҵ����ļ�
string getLastVersion(vector<string> versions, string filename)
{
	string version = ".svn\\commit\\";
	vector<string> contents;
	int i = 0;
	for (; i < versions.size(); ++i)
	{
		contents = getVersionContent(versions[i]);//�汾����־������
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


//��ð汾���ļ��е����� version �汾��
vector<string> getVersionContent(string version)
{
	version = ".svn\\commit\\" + version;
	ifstream infile(version);       //�����ļ�
	vector<string> lines;           //�汾�����ݣ�����
	string line;
	while (getline(infile, line))
	{
		lines.push_back(line);
	}
	return lines;
}

//�ָ��ļ�filename���汾�еģ�
void revertA(string filename, string version_number)
{
	string label;
	label = getLabelInManage(filename);//��manage_file���ҵ��ļ�filename������label
	//û���ҵ��ļ�������label="f"
	if (label == "f")
		return;
	string new_version_file;
	new_version_file = ".svn\\copyfile\\" + version_number + "\\" + filename;

	int flag = 0;
	//��ǩΪC ���޸Ĺ�
	if (label == "C" && diff(filename))
	{
		copyFile(new_version_file, filename);
		flag = 1;
	}
	//�ָ��޸Ļ�ɾ�����ļ����ļ�û�ƶ�����
	if (label == "M" || label=="D")
	{
		//remove(filename.c_str());//�Ƴ�����Ŀ¼���ļ�
		copyFile(new_version_file, filename);
		revertManageFile(filename, label);
		flag = 1;
	} 
	//ɾ������Ŀ¼�±��ƶ����ļ�������newVersion�µ��ļ�������Ŀ¼��manage_file ����versionĿ¼ 
	else if (label == "MV")
	{
		string working_path_name = getWorkingPath(filename); //����Ŀ¼�µ�·��
		//filename�ǰ汾�µ�Ŀ¼��δ���ƶ�����
		remove(working_path_name.c_str());//����Ŀ¼�µ�
		copyFile(new_version_file, filename);
		revertManageFile(working_path_name, label);
		flag = 1;
	}
	if (flag == 0)
		cout << filename << "no need to revert" << endl;
}
//���ļ������ҵ�manage_file�е�label��manage_file�Ҳ������ļ���delete�ˣ�
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
//ͨ��Version�е��ļ�Ŀ¼filename���ҵ� ����Ŀ¼�µ� ·��
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
//MV ����newVersion �µ�Ŀ¼
string getNewVersionPath(string line)
{
	line = line.substr(line.find(" ") + 1);
	line = line.substr(line.find(" ") + 1);
	line = line.substr(0, line.find(" "));
	return line;
}

//����commit version����ļ�����  �ָ�manage_file����һ�汾
//filename Ҫ�ָ����ļ�����
void revertManageFile(string filename, string label)
{
	string manage_path = ".svn\\manage_file";

	vector<string> content;
	readFileIntoVector(manage_path, content);//��manage_file�ļ����ݵ�content
	//����content������filename���� �����labelֵ���������޸�
	for (int i = 0; i < content.size(); ++i)
	{
		//cout << subFilename(content[i]) << endl;
		if (subFilename(content[i]) == filename)
		{
			if (label == "M" || label == "D")
			{
				modifyLabelInFile(manage_path, i + 1, "C");//ֱ�����ļ���ĺ���
			}
			else if (label == "MV")
			{
				
				//ɾ��ǰ����·��������newVersion�е�·��
				deleteOldPath(content[i]);
				content[i] = content[i].substr(content[i].find(" ") + 1);//ɾ����·��

				writeVectorIntoFile(manage_path, content);//��contentд���ļ�

				modifyLabelInFile(manage_path, i + 1, "C");//ֱ�����ļ���ĺ��˱�ǩC
			}
			else
				cout << "can not revert" << filename << endl;
			return;
		}
	}
}

//�ð汾��Ϣ���Ƿ�������ļ�
bool cmpFileInVersion(string version, string filename)
{
	version = ".svn\\commit\\" + version;
	vector<string> contents;
	int i = 0;
	
	contents = getVersionContent(version);//�汾����־������
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