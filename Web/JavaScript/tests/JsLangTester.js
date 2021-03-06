"use strict";
//一些JS知识的网页 -- http://www.cnblogs.com/TomXu/archive/2011/12/31/2289423.html

//★JavaScript区分大小写★
//Netscape中如果多次使用 for 循环向文档重复写 HTML 代码会导致浏览器卡死
//字符串操作没有 StringBuffer 等类型? 通过数组的 join 方法来提高效率

//属性 -- 有两种访问方式, obj.property 和 obj["property"]

module("JSLanguageTester", {
    setup: function() {
    //dump("in JavaScriptLanguage::setup\n");
    },
    teardown : function () {
    //dump("in JavaScriptLanguage::teardown\n");
    }
});


//变量: 只包含字母、数字和/或下划线；区分大小写；要以字母开头；变量需要声明 没有声明的变量不能使用(undefined)
//      JS对数据类型的要求不严格(动态类型)，声明类型可以用赋予初始值的方法做到，但之后可以给变量赋予其他类型的值
//      1.(推荐方式)通过 typeof(变量名) 或 typeof 变量名 的方式可以查询变量的类型，常见的类型有 string, number, array 等
//      2.另一种进行对象类型检测的方法是引用所有对象都有的名为 constructor 的属性
test("变量", function() {
    var varNull;
    console.log("变量: varNull=%o(%s), undefined=%o(%s), null=%o(%s)", varNull, typeof (varNull), undefined, typeof(undefined), null, typeof(null));
    equal(varNull, undefined, "变量未定义或定义之后未赋值，则值为\"undefined\"");
    ok(typeof(varNull) === "undefined", "推荐判断变量是否定义的方法");


    //ok(0144 == 100, "八进制"); //Octal literals are not allowed in strict mode. 
    ok(100 == 100, "十进制");
    ok(0x64 == 100, "十六进制");

    //数值：整形 和 浮点型的 typeof 都是number
    //整数
    var iNumber = 100;
    ok(iNumber == 100, "数值");
    ok(typeof (iNumber) == "number", "整形数据");

    //浮点数 -- 可以用指数计算法表示
    var varFloat = 3.1415926;
    ok(typeof (varFloat) == "number", "浮点型数据");
    equal(-4.1e-8, -0.000000041, "指数表示法表示的浮点数");

    var varBool = true; //false
    ok(typeof (varBool) == "boolean", "纯布尔逻辑值");
    equal(true, 1, "布尔值用于数字表达式时自动变为1和0");
    equal(false, 0);

    var varInfinity = 1 / 0;    //js能处理的最大的数是：1.7976931348623157e+308
    equal(varInfinity, Infinity, "Infinity");  

    //NaN -- Not a Number
    var varNaN = parseInt("abc");
    equal(varNaN.toString(), NaN.toString(), "NaN 字符串比较");
    ok(varNaN != NaN, "NaN直接比较");  
    ok(NaN != NaN, "NaN直接比较"); //注意：NaN 值非常特殊，因为它"不是数字"，所以任何数跟它都不相等，甚至 NaN 本身也不等于 NaN

    var errResult = 123 * "test";  //数字 乘以 字符串 是无法正确转换的
    equal(errResult.toString(), NaN.toString(), "无法正确计算出结果时返回 NaN");


    //使用构造函数属性来判断对象的类型 -- 返回的是对象
    ok(iNumber.constructor == Number, "constructor for Number");
    ok("stringInfo".constructor == String, "constructor for String");
    
    
    //定义只读属性 name 
    //var person = {};
    //Object.defineProperty(person, "name"{ writable: false, value: "fishjam"});
    //equal(person.name, "fishjam", "只读属性");
    //person.name = "another name";   //严格模式下，此行代码会抛出异常
    
});

test("基础语法", function() {
    var iNumber = 2;
    var strNumber = "";

    switch (iNumber) {
        case 0:
            strNumber = "Zero";
            break;
        case 1:
            strNumber = "One";
            break;
        case 2:
            strNumber = "Two";
            break;
        default:
            strNumber = "Unknown";
            break;
    }
    equal(strNumber, "Two", "测试Switch");

    switch (strNumber) {
        case "two":
            iNumber += 1;
            break;
        case "Two":
            iNumber *= 2;
            break;
        default:
            iNumber = 0;
            break;
    }
    equal(iNumber, 4, "switch 也可以判断字符串(区分大小写)");

    //注意：因为 Strict 模式下禁止使用with，因此此处不再测试 with
    //with 为一个或一组语句指定默认对象
    //with (navigator) {
        var strAppName = navigator.appName;
        var strAppCodeName = navigator.appCodeName;

        ok(strAppName != "" && strAppCodeName != "", "测试 with() 语法，能取出对象的数据，不报错");
        //alert(strAppName);
    //}

    ok(1, "this 返回\"当前\"对象。在不同的地方，this 代表不同的对象, 全局代表 window对象");

    equal(1, 1, "TODO: undefined 不是 JavaScript 常数, 不能直接使用 ?");

    //label -- 语句前可冠以标号语句(label)，用于 break 或 continue 语句确定执行程序的转移点,
    //如 label : statement  指明可以跳转的位置后， break statement; 离开标号表示的闭合语句?
    //TODO: 怎么测试 label ?
    var arrayTimes17 = new Array();  //保存能被17整除的数的数组
    for (var i = 1; i <= 100; i++) {
        if (i % 5 == 0) {
            continue;       //跳过5的倍数，这样的话，数组中就不会包含 85(5 * 17)
        }
        if (i % 17 == 0) {
            arrayTimes17.push(i);
        }
    }
    equal(arrayTimes17.length, 4, "测试数组和continue");  //

    //for...in 循环处理对象中定义的属性 -- 可以获取对象的全部属性
    var iCountForIn = 0;
    for (var porp in window) {
        iCountForIn += 1;
        //console.log("%o", porp);
    }
    ok(iCountForIn > 50, "测试 (for in) 获取对象的所有属性");

    //三元运算符( ?: )
    var tripleResult = false ? "TRUE" : "FALSE";
    ok(tripleResult == "FALSE", "三元运算符");
});

//字符串 -- 双引号或单引号中的字符序列，转义字符为 "\"
//可以直接通过 赋值字符串数据来生成变量，也可以通过 new String 的方式来生成变量
test("字符串String", function() {
    var strItem = "origial";
    var strItemRef = strItem;

    strItem += " some info";
    ok(strItemRef != strItem, "修改字符串会创建新的变量");
    ok(strItemRef !== strItem, "修改字符串会创建新的变量");

    var strObj = new String("hello");
    console.log("String test: before %o(%s)", strObj, typeof(strObj));

    var oldStrObj = strObj;
    strObj += " world";
    console.log("String test: after %o(%s) => %o(%s)", oldStrObj, typeof(oldStrObj), strObj, typeof(strObj));
    
    ok(typeof(oldStrObj) == "object", "new String 出来的是 object");
    ok(typeof(strObj) == "string", "进行字符串操作后会转变称string");

    //equal(String.big(strItem), "", "");

    var strAppend = "";
    for (var i = 1; i < 10; i++) {
        strAppend += i;
    }
    equal(strAppend, "123456789", "字符串和数字相加(+)会按字符串连接的方式");

    //调用String对象的方法
    var strObject = new String("百度");
    equal(strObject.anchor('www.baidu.com'), "<a name=\"www.baidu.com\">百度</a>",
        "anchor返回对应的超链接方式，然后可用 document.write 写入文档");
    equal(strObject.link("www.baidu.com"),   "<a href=\"www.baidu.com\">百度</a>", "link 和 anchor 一样");
    
    equal(strObject.charAt(0), "百", "返回指定位置的字符支持Unicode")
    equal(strObject.indexOf("度", 0), 1, "从0开始查找指定字符串");
    equal(strObject.indexOf("网", 0), -1, "没有找到对应的字符串会返回 -1");
    equal(strObject.lastIndexOf("百", 0), 0, "lastIndexOf");

    var strHelloworld = "hello world";
    ok(strHelloworld.length == 11, "length属性 返回字符串的长度");
    ok(strHelloworld.charAt(0) == "h", "CharAt");       //返回该字符串位于第<位置>位的单个字符(Unicode)
    ok(strHelloworld.charAt(strHelloworld.length - 1) == "d", "CharAt(Length-1)");
    equal(strHelloworld.substring(6, strHelloworld.length), "world", "substring获取子串");
    equal(strHelloworld.toUpperCase(), "HELLO WORLD", "toUpperCase返回对应的大写字符串")
    equal(strHelloworld.slice(-5), "world", "slice 从已有的字符串数组中返回选定的元素, (start,end)") //此处取字符串的后5个字符

    //charCodeAt -- 返回该字符串位于第<位置>位的单个字符的 ASCII 码。
    //var strChina = String.fromCharCode();   //返回一个字符串，该字符串每个字符的 ASCII 码由 a, b, c... 等来确定。

    var strHelloArray = strHelloworld.split(" "); //split返回一个数组，该数组是从<字符串对象>中分离开来的
    ok(strHelloArray.length == 2, "split length");
    ok(strHelloArray[0] == "hello", "Split Pos[0]");
    ok(strHelloArray[1] == "world", "Split Pos[1]");

    //通过 fromCharCode 获得对应的Unicode符号(此处是向右的黑色箭头，一般用于播放按钮)
    ok(String.fromCharCode('0x25B6') == String.fromCharCode('0x25B6'), "String get Unicode");

	var formatResult="hello {0}, your are {1}".format("fishjam",30);
	ok(formatResult == "hello fishjam, your are 30", "String Format");
	
    /*
    //系统没有提供 StringBuffer 这个类?
    var strBuf = new StringBuffer();
    strBuf.append("<?xml version='1.0' encoding='utf-8'?>");
    strBuf.append("<tree id='0'>");
    console.log("%o", strBuf);
    */

});

test("运算符", function() {
    //注意运算符优先顺序，也可以用括号调整

    //算术运算符: +,-,*,/(除法),%(取模), ++, --
    equal(17 / 3, 5.666666666666667, "除法");
    equal(Math.floor(17 / 3), 5, "除法以后取整");
    equal(17 % 3, 2, "取模");

    //逻辑运算符: &&, ||, !
    equal(!true, false, "逻辑非");

    //比较运算符: ==, ===, !=, !==,  <, <=, >, >=
    equal(100 == "100", true, "== 和 != 在比较之前会先进行类型转换，然后比较转换之后的值是否相等"); //JavaScript1.2以后才会这样(先转换成公共类型后比较)
    equal(100 === "100", false, "=== 和 !== 会比较值和类型，只有值和类型均相等的时候才会返回true");

    //字符串运算符: + 

    //位操作运算符: &, | , ^, <<, >>(带符号右移), >>>(填0右移)
    equal(0xF1 & 0xE2, 0xE0, "与操作");
    equal(0xF1 | 0xE2, 0xF3, "或操作");
    equal(0xF1 ^ 0xE2, 0x13, "位异或操作");
    equal(0xF1 << 2, 0x3C4, "左移操作");

    //TODO: 用负数来测试 ?
    equal(0xF1 >> 2, 0x3C, "带符号右移操作");
    equal(0xF1 >>> 2, 0x3C, "填0右移操作");

    //带符号右移的话，到了一定的值，再右移也没有用了
    equal(-2 >> 1, -1, "带符号右移操作");
    equal(-2 >> 10, -1, "带符号右移操作");

    equal(-2 >>> 1, 0x7FFFFFFF, "填0右移操作");
    equal(-2 >>> 2, 0x3FFFFFFF, "填0右移操作");

    //赋值运算符: =, +=, -=, *=, /=, %=, >>=, >>>=, &=, |=, ^= 等

    //条件运算符: ?:
    equal(5 < 7 ? true : false, true, "条件运算符");
});

//数组中的数据存在在以 整数 为索引的位置中
test("数组Array", function() {
    var arrayWithLength = new Array(10);
    equal(arrayWithLength.length, 10, "带长度的数组构造函数");
    equal(arrayWithLength[9], null, "此时所有元素的初始值是 null");

    var myArray = new Array("abc", 123);  //带初始值的构造函数(即 "紧凑数组(dense array)"  )
    //等价于 new 之后再如下赋值：
    //  myArray[0] = "abc";
    //  myArray[1] = 123;

   
    var strArrayJoin = myArray.join(" "); //join方法把数组中的各个元素用<分隔符>串起来形成一个字符串，不影响数组内容
    equal(strArrayJoin, "abc 123");

    myArray.reverse();  //逆转数组元素
    strArrayJoin = myArray.join(",");
    equal(strArrayJoin, "123,abc");

    //注意：JavaScript 只有一维数组, 如果要模拟多维数组，需要将数组的元素设置为数组
    myArray[2] = new Array();
    myArray[2][0] = "1000";

    ok(myArray.length == 3, "array length");
    ok(parseInt(myArray[2][0]) == 1000, "array element");

    myArray.push("push info");
    equal(myArray.length, 4, "array push");

    myArray[10] = "new info";
    //console.log("myArray, %o", myArray);
    equal(myArray.length, 11, "直接设置下标 10 的元素，则会自动延伸数组长度，生成 0~10 共11个元素的空间");

    var sitesArray = ["www.baidu.com", "www.google.com", "www.bing.com"];
    equal(sitesArray.length, 3, "直接定义并初始化数组");
    sitesArray.push("www.soso.com");
    equal(sitesArray.length, 4, "动态增加预定义的数组元素");

    //构造数组时直接初始化 -- Schedule 是自定义的类，其构造函数接收 Lecture 数组作为参数
    //var mySchedule = new Schedule([
    //	new Lecture("Math", "Mrs.Jonese"),
    //	new Lecture("English", "TBD"),
    //	]);

    //注意：这种写法不是给数组赋值，而是给数组对象增加了两个属性(one/two)
    var arrayProperty = new Array();
    arrayProperty["one"] = 1;
    arrayProperty["two"] = 2;
    equal(arrayProperty.length, 0);
    equal(arrayProperty["one"] + arrayProperty.two, 3);  //通过取属性的方式进行运算
    //equal(arrayMap["one"], 1);
    //console.log("arrayProperty, %o", arrayProperty);

    //直接定义键值对的数组， 然后在 jQuery 中可以用 $.each( comments , function(commentIndex, comment){ xxx }) 来处理
    //可以直接使用 comment['username'] 和 comment['content'] 来访问每一个元素的属性值。
    //TODO： 处理函数中能自动识别 comment 是 JSON 对象？标准JS没有jQuery的支持也可以同样方式?
    var comments =
    //这些部分放在 .json 文件中就是 JSON 返回方式的数据?
    [
        { "username": "张三", "content": "沙发.", "score": 80 },
        { "username": "李四", "content": "板凳.", "score": 70 },
        { "username": "王五", "content": "地板.", "score": 90 }
    ];
    equal(comments.length, 3, "直接定义键值对数组");
    equal(comments[1].username, "李四", "访问键值对信息");

    var totalScore = 0;
    for( var i = 0; i < comments.length; i++){
        totalScore += comments[i].score;
    }
    //for (var comment in comments) {
    //  totalScore += comment.score;
    //}
    equal(totalScore, 240, "通过for遍历数组元素， TODO: in 语法"); 

});

//比较函数有两个参数，Array.sort 根据其返回值排序
function sortLesser(a, b) {
    return (b - a);
}

test("数组排序", function() {
    var myArray = new Array();
    for (var i = 1; i < 10; i++) {
        myArray[i] = i;
    }
    equal(myArray.join("") ,"123456789");
    myArray.sort(sortLesser);       //按从大到小排序,缺省排序方法时会按照字典顺序排序
    equal(myArray.join("") , "987654321");
});

//注意：Math是内部对象而不是对象类型(即不能创建Math实例) -- 
//var math = new Math();  会抛出"object is not a function"的异常
test("Math", function() {
    //通常在大量的数学运算代码中，使用 with(Math){xxx} 来减少代码

    equal(Math.E, "2.718281828459045", "欧拉常数，是自然对数的底数");
    equal(Math.LN2, "0.6931471805599453", "2的自然对数");
    equal(Math.LN10, "2.302585092994046", "10的自然对数");
    equal(Math.LOG2E, "1.4426950408889634", "以2为底e的对数");
    equal(Math.LOG10E, "0.4342944819032518", "以10为底e的对数");
    equal(Math.PI, "3.141592653589793", "圆周率PI常数");
    equal(Math.SQRT1_2, "0.7071067811865476", "1/2的平方根");
    equal(Math.SQRT2, "1.4142135623730951", "2的平方根");

    equal(Math.ceil(1.234), 2, "ceil 返回大于等于 x 的最小整数");
    equal(Math.floor(1.5), 1, "floor 返回小于等于 x 的最大整数");
    equal(Math.round(1.234), 1, "round 返回 四舍五入后的值"); //即最接近的整数

    equal(Math.min(10, 20), 10, "min 返回较小值");
    equal(Math.max(10, 20), 20, "max 返回较大值");

    equal(Math.abs(-99), 99, "abs返回绝对值")
    equal(Math.exp(2), Math.pow(Math.E, 2), "exp 返回e的X乘方");
    equal(Math.log(20), 2.995732273553991, "返回x的自然对数");
    equal(Math.pow(2, 3), 8, "返回x的y次方");

    //三角函数(cos/sin/tan/acos/asin/atan/atan2)
    //equal(Math.sin(90), 0.8939966636005579, "三角函数 sin/cos 等");
    equal(Math.floor(Math.sin(90) * 1e10), 8939966636, "三角函数 sin/cos 等");

    var myRandomNumber = Math.random() * 40 + 60;
    ok(myRandomNumber >= 60 && myRandomNumber < 100, "random 返回大于 0 小于 1 的一个随机数");
});

test("图像image", function() {
    var img = new Image(); //构造时可以指定 高度、宽度
    img.src = "../HTML/img/merglobe.gif";  //加载指定图像，这种方式加载的图形不能通过 window.document.images 访问
    equal(img.name, "", "图形的name属性");
    equal(img.lowsrc, "", "低速带宽时先加载的图片");

    //加载图像是一个异步过程
    //equal(img.complete, true, "表示图形是否已经装载完成");
    if (img.complete) {
        equal(img.width, 100, "加载的图像的宽度");
        equal(img.height, 100, "加载的图像的高度");
    }
    //equal(img.border, 0, "边框粗细，单位为像素");
    equal(img.hspace, 0, "边界宽度");
    equal(img.vspace, 0, "边界高度");
});

test("Number", function() {
	ok("99" > "100", "字符串比较会使用ASCII序进行比较，而非真正的数值比较");
	ok((+"99") < (+"100"), "前面加'+'号，提示JavaScript里面是数值");
	
    ok(Number.MAX_VALUE < Number.POSITIVE_INFINITY, "最大值 < 正无穷大");
    ok(Number.NEGATIVE_INFINITY < Number.MIN_VALUE, "负无穷大 < 最小值");
});

test("Date", function() {
    var startTime = new Date().getTime();

    //Date日期对象。这个对象可以储存任意一个日期，从 0001 年到 9999 年，并且可以精确到毫秒数（ 1/1000 秒）
    //所有日期时间，如果不指定时区，都采用“UTC”世界时时区
    var d = new Date(2012, 11, 21, 1, 2, 3, 400);
    equal(d.getDate(), 21, "返回Date对象的时间")
    equal(d.getTimezoneOffset(), -480, "中国是东八区，与格林威治时间所差的分钟数为 -480(东方为负)");
    equal(d.getMonth(), 11, "注意月份是从 0 开始的");
    equal(d.toGMTString(), "Thu, 20 Dec 2012 17:02:03 GMT", "获取GMT(格林威治时间)格式的日期型字符串");
    equal(d.toUTCString(), "Thu, 20 Dec 2012 17:02:03 GMT", "获取UTC时间格式的字符串，同GMT?");
    equal(d.toLocaleString(), "2012年12月21日 上午1:02:03", "转换为本地格式，即用户所在地区常用的格式");
    ok(d.getFullYear() == 2012);

    var dParser = Date.parse(d);
    equal(dParser, 1356022923000, "Date.parse 返回该日期对象的内部表达方式(从1970年1月1日零时正开始计算到日期对象所指的日期的毫秒数");
    //equal(d.toString(), dParser.toString(), "直接定义和采用parse 两种方式相等 -- TODO: 为什么不能直接用等的方式？");

    var now = new Date(); //如果不带参数的构造，会使用当前日期和时间创建Date实例
    //equal(now.toLocaleString(), 2012, "");
    ok(now.getFullYear() >= 2013, "当前年>=2013");


    var endTime = new Date().getTime();
    var elapseTime = endTime - startTime;       //返回的值是毫秒单位的时间间隔
    ok(elapseTime < 100);                       //100ms内执行完毕
});

test("TODO:颜色", function() {
//JavaScript定义了许多颜色常量和方法
  equal(1, 1, "TODO:颜色")
 
});

test("TODO: 全局函数", function() {
    equal(4321, eval("432.1 * 10"), "eval把括号内的字符串当作标准语句或表达式来运行, 其中的表达式必须是 数学运算 ?");
    //    try{
    //        eval("hello + world"));
    //        ok(false, "会抛出异常，不会到这里");  //这种方式没有捕获到
    //    }
    //    catch(e)
    //    {
    //        ok(true);
    //    }


    //各种编码方式的区别？哪种才是正确的？函数参数？
    var strSrcURL = "<>[]{}+-=,.;;'\"";             //原始需要编码的字符串

    var strEscapeResult       = "%3C%3E%5B%5D%7B%7D+-%3D%2C.%3B%3B%27%22";      //escape的结果
    var strEncodeUriResult    = "%3C%3E%5B%5D%7B%7D%2B-%3D%2C.%3B%3B'%22";      //encodeURIComponent 的结果
    var strBianma911ChaResult = "%3C%3E%5B%5D%7B%7D%2B-%3D%2C.%3B%3B%27%5C%22"; //http://bianma.911cha.com/ 网站的结果(问题:转义字符?)

    equal(escape(strSrcURL), strEscapeResult, "escape 把字符串按URL编码方法来编码(如空格变为 %20 ),如要对'+'编码，需要加参数(1)");
    equal(unescape(strEscapeResult), strSrcURL, "unescape 解码括号中字符串成为一般字符串");
    equal(encodeURIComponent(strSrcURL), strEncodeUriResult, "encodeURIComponent");
    

    equal(isFinite(parseInt("abc")), false, "isFinite -- 如果括号内的数字是“有限”的(MIN_VALUE和MAX_VALUE之间)就返回true");
    equal(parseInt("123"), 123, "parseInt -- 把括号内的内容转换成整数之后的值,如果第一个支付不是数值，则返回 NaN");
    //equal(parseInt("0123"), 83, "前面有0时默认会按照八进制分析，可能造成错误，但Chrome中实测还是解析成 123");
    equal(parseInt("0123", 10), 123, "parseInt(xxx, 10) -- 强制按照10进制方式分析，★推荐★");
    equal(parseInt("abc").toString(), NaN.toString());
    equal(isNaN(parseInt("abc")), true, "parseInt -- 如果以字母开头，则返回“NaN")

    equal(parseFloat("123.456", 10), 123.456, "parseFloat返回把括号内的字符串转换成浮点数之后的值");

    equal("123".toString(), "123", "toString把对象转换成字符串");
    equal("123".toString(16), "123", "0x7B -- TODO(为什么不行？):toString(N)可以转换成特定进制");
    
    
});

test("自定义函数", function() {
    //1.函数必须先定义后使用，
    //2.函数中可以用自动生成的参数 arguments 数组处理传入的参数,使用时可以加 函数名前缀.
    //3.声明变量时，若前面有 "var" 则表明是局部变量，否则就是全局变量（即使是在函数内部，但需要调用过函数后才会生成全局变量）

    function SomeFun() { //此处最好写上参数列表 -- 方便函数使用者知道调用方式
        checkParams("SomeFun", arguments);
        
        equal(arguments.length, 4, "函数中的 arguments 数组参数");
        //equal(SomeFun.arguments.length, 4, "函数名前缀.arguments"); //Strict模式下不能访问 'caller', 'callee', and 'arguments 等参数
        //SomeFun.caller;
        //SomeFun.callee;

        strictParams([Number, String, Date, String], arguments);  //使用 Utils 中定义的辅助函数验证输入的参数
        for (var i = 0; i < arguments.length; i++) {
            //alert(arguments[i]);
        }

        //函数内部声明变量时，前面没有加 "var"， 则会是全局变量
        //tmpGlobalVariable = 100;  //如果启用这句，且 Qunit 中选中"Check for Globals", 则会显示这是全局变量("Introduced global variable")
    }
    var funReturn = SomeFun(100, "abc", new Date(), new String("abcde"));

    console.log("typeof(SomeFun) = %s, funReturn=%o(%s)", typeof(SomeFun), funReturn, typeof(funReturn));
    ok(typeof(SomeFun) === "function", "函数对象");
    //ok(typeof(funReturn) === "undefined", "");
    ok(funReturn === undefined, "JavaScript的函数一定有返回值，如果没有显式返回，则返回undefined");

    //一个接受任意数量参数并将其转换为数组的自定义函数
    function makeArray() {
        //定义临时使用的数组
        var arr = [];
        //遍历传入的每个参数，并加入数组，然后返回
        for (var i = 0; i < arguments.length; i++) {
            arr.push(arguments[i]);
        }
        return arr;
    }

    //函数生成器：一个返回函数的函数
    //  利用闭包的特性，返回一个简单的求两个数字和的函数，外面可以使用
    function addGenerator(num) {
        return function(toAdd) { return num + toAdd; }
    }

    //自动执行的匿名函数 -- 自调用函数(self-invoking functions)
    (function() {
        var msg = "自动执行的匿名函数，这个函数可以使用原来必须写成全局的变量(特指 window.onunload = xxx 部分?)";
        //window.onunload = function() {
            //绑定函数到全局对象，并使用“隐藏”的msg变量
        console.log(msg);
        //};
    })();

    //TODO:具体的参数是什么?
    equal(arguments.length, 1, "TODO:函数内部可以用 arguments 数组属性来获得外部程序调用函数时指定的参数信息");
    //这样可以在函数中获取任意多个参数 -- 不过对开发来说，可读性降低，只在调试时使用?
    //注意：1.arguments参数不能修改; 2.如果对形参没有提供实参，也能编译过，但 typedef 参数名 == "undefined" 

    equal(makeArray("a", 1, 10.2).length, 3, "使用 arguments 参数的函数");

    var addFive = addGenerator(5);
    equal(addFive(4), 9, "函数生成器");
});

test("具有变参的函数", function() {
    function someVariableFun(paramObj) {
        console.log("someVariableFun=%o", paramObj);
        equal(paramObj["name"], "testVariableFun", "属性名取参数");
        equal(paramObj.value1, "testValue1", "");
        equal(paramObj.value2, "testValue2");
        if(paramObj.optionValue != null){
            equal(paramObj.optionValue, "this is option Value", "可选参数");
        }

        return paramObj.name;
    }

    //调用方式1: 
    var callParamObj = new Object();
    callParamObj["name"] = "testVariableFun";
    callParamObj["value1"] = "testValue1";
    callParamObj["value2"] = "testValue2";
    equal(someVariableFun(callParamObj), "testVariableFun", "生成指定的变量设置属性来调用变参函数");
    
    
    //调用方式2:
    var checkResult = someVariableFun( {
        "name":"testVariableFun",
        value1 : "testValue1",
        value2: "testValue2",
        optionValue : "this is option Value",
    });
    equal(checkResult, "testVariableFun", "使用匿名对象的方式来调用变参函数");

    //调用方式2等价于：
    var anotherCallParamObj = {
        name:"testVariableFun",
        value1:"testValue1",
        value2: "testValue2",
        optionValue : "this is option Value",
    };
    equal(someVariableFun(anotherCallParamObj), "testVariableFun", "生成变量再调用");
});

//指定事件处理程序的方法
//1.直接在 HTML 标记中指定, onload="xxx" onunload="xxx"
//2.编写特定对象特定事件的 JavaScript，通常很少用。
//  如：<script language="JavaScript" for="对象" event="事件">事件处理程序代码 </script>
//3.在 JavaScript 中说明, <对象>.<事件> = <事件处理程序>; 如 window.onerror = globalError; 
test("TODO: 事件处理程序", function() {

    ok(1, "TODO:事件处理程序的返回值(类似beHandled)：false表明需要系统继续处理；true表明已经处理完毕");
});


function globalError(msg, url, lineno) {
    alert("window.onerror\n\n" + "Error: " + msg + "\n" + "URL:  " + url + "\n" + "Line:  " + lineno);
    return true;
}

//指定全局的错误处理函数
//window.onerror = globalError; //注意没有最后的括号"()"
//返回值：
//  true -- 表示已经处理
//  false --
test("TODO: 错误处理程序", function() {
    
    ok(1, "TODO");
});

function someFun() {
    var localVariable = 100;    //加了var的是局部变量
    //g_varInSomeFun = 10;        //不加var的是全局变量  -- Strict 模式下不生效
}

var g_myGlobalVariable = "some value";  //此处加不加var都一样是全局变量
test("作用域", function() {
    //JavaScript的变量的作用域不是块（block scope）而是函数(function scope)
    equal(window.g_myGlobalVariable, "some value", "全局作用域的变量都是window对象的属性");

    someFun();   //必须先调用一次该函数，才能使用其内部生成的全局变量。TODO：如果多次调用会如何?
    //equal(g_varInSomeFun, 10, "定义在函数内部的全局变量，需要调用函数后才会生效");
});

function delayCheckString(msg, time) {
	setTimeout(function() {
		//alert(msg == "some info");   //这个alert会在一秒以后弹出 "true"
		equal(msg, "some info", "通过闭包特性，函数能访问已经结束的函数中的变量");
	}, time);
}

/***********************************************************************************************************
* 闭包:简单理解就是 "定义在一个函数内部的函数，该函数能够读取其他函数(父函数?)的内部变量"
*      本质上 闭包就是将函数内部和函数外部连接起来的一座桥梁
* 用途: 
*   1.读取函数内部的变量; 
*   2.让这些变量的值始终保持在内存中(闭包被保存成变量，其依赖的外部函数即使生存期结束，也会保留在内存中)
* 注意：
*   1.由于闭包会使得函数中的变量都被保存在内存中，内存消耗很大，所以不能滥用闭包，否则会造成性能问题，且可能导致内存泄露.
*     解决：在退出函数之前，将不使用的局部变量全部删除
*   2.闭包会在父函数外部，改变父函数内部变量的值。如当把父函数当作对象(object)使用，把闭包当作它的公用方法(Public Method)，
*     把内部变量当作它的私有属性(private value)时，不要随意改变傅函数内部变量的值
***********************************************************************************************************/

test("闭包", function() {
	//delayCheckString("some info", 1000);
    equal(1, 1, "闭包测试");
	
});
