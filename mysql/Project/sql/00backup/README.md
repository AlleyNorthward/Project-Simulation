&emsp;&emsp;@author 巷北  
&emsp;&emsp;@time 2026-01-01 20:55:11  

# 备份命令

~~~sql
mysqldump --single-transaction --set-gtid-purged=OFF -u root -p hotel_management > hotel_management_backup.sql
~~~

# 恢复命令

~~~sql
cmd
mysql -u root -p hotel_management < hotel_management_backup.sql
exit
~~~
