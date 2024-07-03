#! /bin/sh

# Deploys modified UDF code 
sudo rm mysql_cmdshell.c
sudo ./generate_mysql_cmdshell.c.sh
sudo chmod 750 mysql_cmdshell.c

sudo gcc -shared -o mysql_cmdshell.so -fPIC -DMYSQL_DYNAMIC_PLUGIN -std=c99 mysql_cmdshell.c $(mysql_config --cflags) $(mysql_config --libs)

sudo chmod 750 mysql_cmdshell.so 
sudo cp mysql_cmdshell.so /usr/lib64/mysql/plugin/

sudo chmod 644 /usr/lib64/mysql/plugin/mysql_cmdshell.so
sudo chown mysql:mysql /usr/lib64/mysql/plugin/mysql_cmdshell.so

sudo systemctl restart mysql

