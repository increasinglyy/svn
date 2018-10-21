## 一些记录

### create

`svn create`

提示：`Create an empty Svn repository`

新建`.svn`、`.svn\commit`、`.svn\copyfile`、`.svn\copyfile\newVersion`文件夹，`.svn\manage_file`文件



### status

`svn status`

modified（如果是标记C的文件，要diff一下是不是要把标签改为M）

没有create输入该命令，应该提示些什么



如果强行在工作目录被删掉（丢失），提示G



### add +

`svn add aadd.txt`

提示：1、`add aadd.txt to repo`

​	   2、如果之前已经add过（在manage_file中有记录），则添加无效，提示：`xxx is managed`

​	   3、输入的文件名不在目录中，提示： `xxx is not in directory` ~~（输入的文件前后不能多加空格！！！之后改进）~~

操作：manage_file文件中添加一行 `aadd.txt 1539588959 1539588981 +`



### commit C

`svn commit aadd.txt -m 'first commit'`

`svn commit ffff\f1.txt -m 'commit f'`



提示：`commit success，version number is : 1539651499` 成功的

​	   `xxx no modification` 该文件没有改动（和newVersion中的一样）

​	   `xxx did not add to repo` xxx文件没有在manage_file中

​	    `Please inter file or directory name：svn commit [文件名/目录名] (-m '提交日志')` 没有输入文件名，直接-m

​	   	  

操作：

> 检查该文件是否在仓库，若在 是否标记+ M D MV（如若标记C，检查是否修改，如果修改了 commit成功，时间信息没改过来!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!）
>
> 如果没有-m的提交日志，记录文件第一行为空

1. 将manage_file中commit的文件标记 C
2. 创建记录文件：（以版本递增号作为文件名 如1539589159）于commit目录下 ，文件储存。①提交日志 `first commit`，②commit前的文件名 时间信息 标记  `aadd.txt 1539781553 1539781553 +` ③ commit后的文件名 时间信息 标记 `aadd.txt 1539781553 1539781553 C`
3. 备份1： copyfile文件夹下新建版本递增号文件夹，内备份 commit后的文件（文件为工作目录下的文件）

4. 备份2： newVersion文件夹 内备份 commit后的文件（文件为工作目录下的文件）




```
commit需要 检查该文件在manage_file中是否存在，是否标记+ M D MV，是的话 改为C

如果commit之后（清除原来标记，标记为C），
如果对commit之后的文件进行修改，用户自己改了 但标记还是C， 所以要检查标记为C的文件（diff），即 如果和newversion中的文件内容不同，则要标记为M
最后再commit

如果是标记为D，commit后直接删除（删掉newVersion下的文件）

如果是标记MV的文件，要删除其余的目录（上一次和newVersion），要删除newVersion下的文件，标记为C，备份新的到newVersion下（从manage_file中读出路径文件）。（原来的目录删不掉？？？？）
```







### delete D

`svn delete aadd.txt`

`svn delete bbdd.docx --force`

提示：

`aadd.txt delete` 文件删除成功

`and delete cource file` --force 删除源文件

`delete fail, aadd.txt did not been managed` 文件未被托管，删除失败

`+ status can not been delete, you should commit before del`

操作：

将一个受管理的文件`aadd.txt`纳出管理状态（不删除物理文件）--->~~删除manage_file信息，newVersion中的备份~~

--->将manage_file中该文件标记为D，（不删除newVersion中的信息，   commit之后才删除）

> `svn commit aadd.txt -m 'delete aadd'`（将一个delete的文件commit）
>
> 提示：`commit success, version number is : 1539786546`
>
> 操作：
>
> manage_file中删了这一行（移出管理）
>
> newVersion中删除该文件
>
> 备份commitNum文件（不备份到NewVersion）

> 标记为C的文件可以被delete
>
> 标记为M的 或者其他的不可以



如果未被管理，则无任何效果，通过--force删除实际文件



note：不是标记为C的文件不能delete



### mv MV

`svn mv aadd.txt ffff`

提示：

`path changed : aadd.txt -> ffff\aadd.txt` ！保存路径更改的

`aadd.txt did not managed`



操作：

1、移动文件（工作目录下），

2、添加仓库记录manage_file的 path（新、前一个，newVersion中），标记MV

`svn mv ccdd.pptx a` --->

`a\ccdd.pptx ccdd.pptx ccdd.pptx 1540095546 1540095564 MV`

（newVersion中不更新， commit之后才要更新， ）

> 这时候commit：`svn commit a\aadd.txt -m 'move'`
>
> 提示：`commit success, version number is : 1539777324`
>
> 操作：
>
> manage_file中，改为`a\aadd.txt 1539699924 1539699924 C`
>
> newVersion中，删除原来的aadd.txt，添加a\aadd.txt
>
> commit文件、copyfile的版本文件



移动到当前目录

> `svn mv a\ccdd.pptx .`
>
> 提示：`path changed: a\ccdd.pptx -> .\ccdd.pptx`
>
> 操作：
>
> 工作目录下，a\ccdd.pptx -> .\ccdd.pptx
>
> manage_file中，改为了`ccdd.pptx a\ccdd.pptx ccdd.pptx 1540095546 1540095564 MV`
>
> ~~但是，commit之后提示`aadd.txt did not add to repo`（因为在manage_file中找不到aadd.txt~~



revert????



### git log

`svn git log`

提示：`version number is: 1539777011, log: first aadd.txt`

操作：读取 commit 文件夹下内容

`svn git log aadd.txt`



### revert



```
//svn revert (版本号) (-f 文件名/目录名) ---> 恢复当前文件到历史版本中
/*如果不设置版本号和文件名，则默认将仓库托管的所有文件恢复到上一提交版本
**如果不设置版本号而设置文件名，则将特定文件恢复到上一版本
**如果设置版本号而不设置文件名，将仓库托管的所有文件恢复到特定提交版本
**如果既设置版本号又设置文件名，则将特定文件恢复到特定提交版本
*/
```

#### 1、`svn revert`  

如果不设置版本号和文件名，则默认将仓库托管的所有文件恢复到上一提交版本

只能revert  manage_file中 标记为 M(检查C) D MV 的文件，

#### 2、`svn revert rd.txt`

如果不设置版本号而设置文件名，则将特定文件恢复到上一版本



如果是退回某个版本，只退回版本中的文件

------



### 问题

1. 如果加入的是一个文件夹，如何把该文件夹下的所有文件全部添加（add\commit\revert...)

2. create一个目录a  `svn create a`

3. 对于删除修改文件内容，只能将文件内容全部读出，改完在重新写入文件，对文件的操作？？？

4. 每次都要运行main.cpp

5. delete --force 后 commit ，不能revert

   只能revert  manage_file中 标记为 M(检查C) D MV 的文件????





### 解决

（如果之前已经commit aadd.txt  再 `svn commit aadd.txt -m 'z'` 报错: `vector subscript out of range`）

1. diff返回1错。文件没有改，应该返回0

   `v1.size()` 和 `v2.size()` 相等的情况没有考虑

2. `modifyContentInFile(".svn\manage_file", line, "M");` 错。`vector subscript out of range`

   应该是`.svn\\manage_file`

3. 如果create后直接status（manage_file中没有信息），则status命令无输出。

4. delete --force 后 commit 备份文件为空（工作目录下已经被删了）
