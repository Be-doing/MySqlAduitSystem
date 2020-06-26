String.prototype.trim = function(){
    return this.replace(/(^\s*)|(\s*$)/g, "");
}
String.prototype.isValidTime=function() { 
	var result=this.match(/^(\d{1,2}):(\d{1,2}):(\d{1,2})$/); 
	if (result==null) return false; 
	if (result[1]>24 || result[2]>60 || result[3]>60) return false; 
	return true; 
}
String.prototype.isValidDate=function() { 
	var result=this.match(/^(\d{1,2})\-(\d{1,2})$/); 
	if(result==null) return false; 
	var d=new Date(2020, result[1]-1, result[2]); 
	return (d.getFullYear()==2020&&d.getMonth()+1==result[1]&&d.getDate()==result[2]);
}
function checkTime(timestr) {
	var str = timestr.trim();
	if(str.isValidTime()==false){
		alert("请输入格式正确时间期\n\r时间格式：HH:MM:SS\n\r例    如：13:14:00\n\r");
		return false;
	}
}
function checkDate(timestr) {
	var str = timestr.trim();
	if(str.isValidDate()==false){
		alert("请输入格式正确的日期\n\r日期格式：yyyy-mm-dd\n\r例    如：2008-08-08\n\r");
		return false;
	}
}

function checkip(ip){
    var pcount = 0
    var ip_length = ip.length
    var ip_letters = "1234567890."
    for (p=0; p < ip_length; p++) {
		var ip_char = ip.charAt(p)
		if (ip_letters.indexOf(ip_char) == -1){
			return false
		}
	}
	for (var u = 0; u < ip_length; u++) { 
		(ip.substr(u,1) == ".") ? pcount++ : pcount 
	}
	if(pcount != 3) { 
		return false
	}
	firstp = ip.indexOf(".")
	lastp = ip.lastIndexOf(".")
	str1 = ip.substring(0,firstp)
	ipstr_tmp = ip.substring(0,lastp)
	secondp = ipstr_tmp.lastIndexOf(".")
	str2 = ipstr_tmp.substring(firstp+1,secondp)
	str3 = ipstr_tmp.substring(secondp+1,lastp)
	str4 = ip.substring(lastp+1,ip_length)
	if (str1 == '' || str2 == '' || str3 == '' || str4 == '') { return false }
	if (str1.length > 3 || str2.length > 3 || str3.length > 3 || str4.length > 3) { return false }
	if (str1 <= 0 || str1 > 255) { return false }
	else if (str2 < 0 || str2 > 255) { return false }
	else if (str3 < 0 || str3 > 255) { return false }
	else if (str4 < 0 || str4 > 255) { return false }
	return true
}

function checkIP(ip){
	var ipstr = ip.trim();
	if(checkip(ipstr)==false){
		alert("请输入格式正确的IP地址\n\r日期格式：xxx.xxx.xxx.xxx\n\r例    如：192.168.1.1\n\r")
	}
}

function lastalert(){
	return confirm("请保证开始值小于或等于结束值\r\n(如果需要设置相同的值，则将开始值和结束值设置成一样");
		
}