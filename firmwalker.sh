set -e
set -u
# 防止输出中文乱码
export LANG=zh_CN.UTF-8

# 使用方法
function usage {
    echo "   firmwalker   "
	echo "使用方法："
	echo "$0 {已解压的固件文件路径}\
 {可选：文件保存的地址，默认为firmwalker.txt}"
	echo "例如： ./$0 linksys/fmk/rootfs/"
	exit 1
}

function msg {
    echo "$1" | tee -a $FILE
}

function getArray {
    array=()
    while IFS= read -r line
    do
        array+=("$line")
    done < "$1"
}

# 检测命令行内容，如果只有1个参数或大于3个参数，则输出使用方法并退出
if [[ $# -gt 2 || $# -lt 1 ]]; then
    usage
fi

# 设置变量
FIRMDIR=$1
if [[ $# -eq 2 ]]; then
    FILE=$2
else
    FILE="firmwalker.txt"
fi
# 如果原先有相应的文件，将其移除
if [[ -e "$FILE" && ! -h "$FILE" && -f "$FILE" ]]; then
    rm -f $FILE
fi

# 进行搜索
msg "***固件地址***"
msg $FIRMDIR
msg "***正在搜索密码文件***"
getArray "data/passfiles"
passfiles=("${array[@]}")
for passfile  in "${passfiles[@]}"
do
    msg "##################################### $passfile"
    find $FIRMDIR -name $passfile | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg "***正在搜索Unix-MD5哈希值***"
egrep -sro '\$1\$\w{8}\S{23}' $FIRMDIR | tee -a $FILE
msg ""
if [[ -d "$FIRMDIR/etc/ssl" ]]; then
    msg "***正在枚举etc/ssl文件夹下的内容***"
    ls -l $FIRMDIR/etc/ssl | tee -a $FILE
fi
msg ""
msg "***正在搜索SSL相关文件***"
getArray "data/sslfiles"
sslfiles=("${array[@]}")
for sslfile in ${sslfiles[@]}
do
    msg "##################################### $sslfile"
    find $FIRMDIR -name $sslfile | cut -c${#FIRMDIR}- | tee -a $FILE
       certfiles=( $(find ${FIRMDIR} -name ${sslfile}) )
       : "${certfiles:=empty}"
       # 进行Shodan搜索。假设已经通过API秘钥安装了Shodan CLI。
       if [ "${certfiles##*.}" = "pem" ] || [ "${certfiles##*.}" = "crt" ]; then
          for certfile in "${certfiles[@]}"
          do
             serialno=$(openssl x509 -in $certfile -serial -noout) || echo "不正确的文件内容：继续"
             serialnoformat=(ssl.cert.serial:${serialno##*=})
             if type "shodan" &> /dev/null ; then
                 shocount=$(shodan count $serialnoformat)
                 if (( $shocount > 0 )); then
            		msg "################# 在Shodan中发现的证书序列号 ####################"
            		echo $certfile | cut -c${#FIRMDIR}- | tee -a $FILE
            		echo $serialno | tee -a $FILE
                         	echo "在Shodan中发现" $shocount | tee -a $FILE "个设备"
            		cat $certfile | tee -a $FILE
            		msg "###########################################################################"
                 fi
             else 
                echo "未找到Shodan CLI"
             fi
          done
       fi
    msg ""
done

msg ""
msg "***正在搜索SSH相关文件***"
getArray "data/sshfiles"
sshfiles=("${array[@]}")
for sshfile in ${sshfiles[@]}
do
    msg "##################################### $sshfile"
    find $FIRMDIR -name $sshfile | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg ""
msg "***正在搜索files文件夹***"
getArray "data/files"
files=("${array[@]}")
for file in ${files[@]}
do
    msg "##################################### $file"
    find $FIRMDIR -name $file | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg ""
msg "***正在搜索数据库相关文件***"
getArray "data/dbfiles"
dbfiles=("${array[@]}")
for dbfile in ${dbfiles[@]}
do
    msg "##################################### $dbfile"
    find $FIRMDIR -name $dbfile | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg ""
msg "***正在搜索shell脚本***"
msg "##################################### shell脚本"
find $FIRMDIR -name "*.sh" | cut -c${#FIRMDIR}- | tee -a $FILE
msg ""
msg "***正在搜索其他.bin文件***"
msg "##################################### bin文件"
find $FIRMDIR -name "*.bin" | cut -c${#FIRMDIR}- | tee -a $FILE
msg ""
msg "***正在搜索文件样式***"
getArray "data/patterns"
patterns=("${array[@]}")
for pattern in "${patterns[@]}"
do
    msg "-------------------- $pattern --------------------"
    grep -lsirnw $FIRMDIR -e "$pattern" | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg ""
msg "***正在搜索网络服务器***"
msg "##################################### 网络服务器"
getArray "data/webservers"
webservers=("${array[@]}")
for webserver in ${webservers[@]}
do
    msg "##################################### $webserver"
    find $FIRMDIR -name "$webserver" | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done
msg ""
msg "***正在搜索重要二进制文件***"
msg "##################################### 重要二进制文件"
getArray "data/binaries"
binaries=("${array[@]}")
for binary in "${binaries[@]}"
do
    msg "##################################### $binary"
    find $FIRMDIR -name "$binary" | cut -c${#FIRMDIR}- | tee -a $FILE
    msg ""
done

msg ""
msg "***正在搜索IP地址***"
msg "##################################### IP地址"
grep -sRIEho '\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b' --exclude-dir='dev' $FIRMDIR | sort | uniq | tee -a $FILE

msg ""
msg "***正在搜索URL***"
msg "##################################### URL"
grep -sRIEoh '(http|https)://[^/"]+' --exclude-dir='dev' $FIRMDIR | sort | uniq | tee -a $FILE

msg ""
msg "***正在搜索邮箱***"
msg "##################################### 邮箱"
grep -sRIEoh '([[:alnum:]_.-]+@[[:alnum:]_.-]+?\.[[:alpha:].]{2,6})' "$@" --exclude-dir='dev' $FIRMDIR | sort | uniq | tee -a $FILE

msg ""
msg "搜索完毕"

#Perform static code analysis 
#eslint -c eslintrc.json $FIRMDIR | tee -a $FILE
