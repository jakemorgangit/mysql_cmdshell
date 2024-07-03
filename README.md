*mysql_cmdshell Project*

1) Clone repo
2) Modify allow.list to include any additional shell commands not already listed (use with extreme caution!)
3) Run deploy.sh
4) Launch a connection to MySQL / MariaDB and run the following commands:
   
   `DROP FUNCTION IF EXISTS mysql_cmdshell;`
   
   `CREATE FUNCTION mysql_cmdshell RETURNS STRING SONAME 'mysql_cmdshell.so';`


Now you can run shell commands directly from MySQL:


**Check CentOS Version:**   

    SELECT mysql_cmdshell('cat /etc/centos-release');
    
**Kernel Version:**    

    SELECT mysql_cmdshell('uname -r');
    
**System Architecture:**    

    SELECT mysql_cmdshell('uname -m');
    
**Hostname:**     

    SELECT mysql_cmdshell('hostname');
    
**IP Address:**    

    SELECT mysql_cmdshell('ip addr show');

**CPU Information:**

  `SELECT mysql_cmdshell('lscpu ');`
  
  
  `SELECT mysql_cmdshell('lscpu | grep name');`
  
  
  `SELECT mysql_cmdshell('lscpu | grep MHz');`
  
  `SELECT mysql_cmdshell('lscpu | grep MHz');`
  
  `SELECT mysql_cmdshell('lscpu | grep "CPU(s):"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "On-line"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "Thread"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "Core(s) per socket"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "Socket(s):"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "NUMA node(s)"');`
  
  `SELECT mysql_cmdshell('lscpu | grep "NUMA"');`

**Memory Information:**  

  `SELECT mysql_cmdshell('free -m');`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $2}'") as MemoryTotal;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Swap:' | awk '{print $2}'") as SwapTotal;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $3}'") as MemoryUsed;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Swap:' | awk '{print $3}'") as SwapUsed;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $4}'") as MemoryFree;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Swap:' | awk '{print $4}'") as SwapFree;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $5}'") as MemoryShared;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $6}'") as MemoryBufferCache;`
  
  `SELECT mysql_cmdshell("free -m | grep 'Mem:' | awk '{print $7}'") as MemoryAvailable;`

**System Start**

  `SELECT mysql_cmdshell('uptime');`
  
  `SELECT mysql_cmdshell('who -b');`

**View System Logs:**  

   `SELECT mysql_cmdshell('journalctl | grep error');`

**SE Linux:** 

  `SELECT mysql_cmdshell('sestatus | grep "SELinux status"');`
  
  `SELECT mysql_cmdshell('sestatus | grep "Current mode"');`
