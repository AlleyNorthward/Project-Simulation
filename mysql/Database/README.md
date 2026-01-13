&emsp;&emsp;@author 巷北  
&emsp;&emsp;@time 2025-12-30 12:32:04  

# 简介
&emsp;&emsp;这里专门存储一些必要的命令吧, 方便可是化. 为了避免之前杂乱无章, 毫无层次感地写命令的方式, 这里呢, 首先要做的就是结构清晰, 能够很迅速地查找到自己想用的命令在哪里. <br>

- [编写风格](#编写风格)
    - [大小写约定](#大小写约定)
- [理解](#理解)
- [基础查看](#基础查看)
    - [系统](#系统)
        - [数据存储位置](#数据存储位置)
    - [用户](#用户)
        - [查看所有用户](#查看所有用户)
        - [查看权限](#查看权限)
    - [模式](#模式)
    - [表](#表)
- [创建与删除](#创建与删除)
    - [用户类](#用户类)
        - [创建用户](#创建用户)
        - [删除用户](#删除用户)
    - [模式类](#模式类)
        - [创建模式](#创建模式)
        - [删除模式](#删除模式)

# 编写风格
## 大小写约定

像`SELECT`等的这些关键字, 默认就大写. 像表内某些字段, 比如`User`, 这些就首字母大写, 像`schema.table`, 这些就直接小写. 目前来看, 就这么多吧, 不多说了. 

# 理解
- 下面写了一些. 结合之前自己的理解呢, 逐渐有些领悟了. 不论我如何查看, 查看什么, 它似乎总是一张表. 所以呢, 这就变成了如何灵活查看这张表. 那么问题就变得很简单很简单. 
    - 首先, 需要确定是哪张表. 比如, 我先查看权限, 这个表在`information_schema.user_privileges`中. 
    - 之后, 确定用户, 比如`'root@localhost'`, 在表中对应的属性是`Grantee`
    - 这样其实就行了. 
        - 接下来, 选中想查看的属性. 比如, 所有属性我都想查看. 那么就是`SELECT *`
        - 然后, 确定范围表, `FROM information_schema.user_privileges`.
        - 之后, 约束行, 也就是`WHERE Grantee = "'root'@'localhost'"`
    - 总结来看, 就是`SELECT`是约束列的, 而后面的限定, 是约束行的. 
    - 所以, 我们可以先查看这个表的所有信息, 然后呢, 就直接选择去看就行了. 
    - 那么, 我们下面说的所有的查看方式, 其实都没有任何必要了, 只需要知道是哪张表, 外加对应属性含义, 就能找到所有我们要找到的信息了. 
    - 所以, 我把所有命令行删除了. 


# 基础查看
## 系统
### 数据存储位置

~~~sql
SHOW VARIABLES LIKE 'datadir';
~~~

**解释**: 这里呢, `VARIABLES`中文是变量的意思. 在数据库这里呢, 就是指`MYSQL`的配置项. 可以查看所有配置项, 也就是`SHOW VARIABLES;`. 后面跟着的`LIKE 'datadir'`则代表, 仅查看这一个配置项, 这样我们就能看到数据库中的数据保存在哪个盘中了. 因为数据库配置很久了, 我想看看数据是否存储在`C`盘中, 所以这么看看. 

## 用户
### 查看所有用户

`mysql.user`

### 查看权限

~~~sql
SHOW GRANTS FOR 'user1'@'localhost';
~~~

`information_schema.user_privileges`

这个呢, 是查看某个用户的权限的. 

## 模式

~~~sql
SHOW DATABASES;
~~~

`information_schema.schema`

## 表

~~~sql
SHOW TABLES
FROM information_schema;
~~~

`information_schema.tables`

# 创建与删除
## 用户类

### 创建用户

- 普通用户
~~~sql
CREATE User 'user1'@'localhost' IDENTIFIED BY 'user1';
~~~

### 删除用户

~~~sql
DROP USER 'user1'@'localhost';
~~~

## 模式类
### 创建模式

~~~sql
CREATE Database test;
~~~

~~~sql
CREATE Database demo
CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci;
~~~

### 删除模式

~~~sql
DROP Database test;
~~~
