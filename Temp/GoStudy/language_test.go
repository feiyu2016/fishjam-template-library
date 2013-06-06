//http://coolshell.cn/articles/8489.html

//TODO:
//1.go是静态类型的语言 -- 什么意思? 对应的还有动态类型的语言?

/*******************************************************************************
定义变量(两种方式)
  1.var 变量名 [类型][= 初始值] -- 语法很像javascript(可不指明类型，通过初始化值来推导)
  2.变量名 := 初始值

常见类型(各类型全部独立，混用这些类型向变量赋值会引起编译器错误 -- 强制转换: type(value)?)：
  bool --
  byte -- uint8 的别名
  complex64(32位虚数部分)/complex128(64位虚数部分) -- 复数
  int/uint -- 32位系统是32位，但64位系统是64位的?
  int32/uint32/int64/uint64/float32/float64
  string -- 字符串是常量，是不能修改的？ 即 string[0] = 'S' 是非法的？
常量 const ( 常量名 [类型] = 常量值 ),可通过 iota 来定义枚举
特殊变量名(_, 下划线)，任何赋给它的值都被丢弃，通常用于不关心的变量，否则会出现编译错误(声明了变量却不使用)

[]定义了slice, [size]定义了数组，*定义了指针

执行程序的入口点 func 是 main
import "路径" -- import指定路径里的所有go文件(排除 _test.go)

注意：
 1.无论任何时候，你都不应该将一个控制结构（(if、for、switch或select）的左大括号放在下一行。
   如果这样做，将会在大括号的前方插入一个分号，这可能导致出现不想要的结果。
 2.Go的正式的语法使用分号来终止语句，但这些分号由词法分析器在扫描源代码过程中使用简单的规则自动插入分号，源码中就不需要分号了。
 3.通常Go程序仅在for循环语句中使用分号，以此来分开初始化器、条件和增量单元
 4.没有 public/protected/private 的关键字，如果想让方法可以被别的包访问的话，第一个字母必须大写。

内存分配原语 -- 两种内存分配方式区别的原因是 slice,map,chan 使用前必须初始化内部数据结构(非零)
  new(T)--分配内存的内建函数,返回指针，只是将内存清零，而不是初始化内存，
    new(T)返回一个指向新分配的类型为T的零值的指针。
	即使用者可用new创建一个数据结构的实例并且可以直接工作
  make(T, args)--仅用于创建内建的slice、map和chan(消息管道),
    并返回一个被初始化了的(不是零)类型T(不是*T)实例

指针 -- Go有指针，但没有指针运算(从使用上来说，更像引用)，取址操作符(&)，取值操作符(*)

切片 -- 一个具有三项内容的描述符，包括指向数据（在一个数组内部）的指针、长度以及容量
函数 -- func [p 特定类型] 函数名(变量名 变量类型 , ...) (返回值 返回类型,...) { 函数体 }
 可以随意安排函数的顺序 -- 不需要前向声明函数原型
 可选的"特定类型"表示这是特定类型的成员方法(参见 class_test )
 返回多值 -- 很多函数都会返回两个值，一个正常值，一个是错误
 函数不定参数(变参) -- func sum(nums ...int)
 函数作为值赋值给变量(相当于函数指针),
   pFun:=func(){xxx}  //定义匿名函数并赋值给变量
   pFun()		      //通过变量调用函数

函数闭包 -- 返回匿名函数的函数？
   func plusX(x int) func(int) int { return func(y int) int { return x + y; }
   闭包可以毫不费力的将局部变量传递到回调

关键字
  fallthrough -- 用于switch...case中，当匹配失败后自动向下尝试(类似C中没有break时?)
  range--迭代器，可以从array,slice,string,map,channel等需要循环的内容中返回一个键值对
  select(选择不同类型的通讯),chan,
  switch -- 没有break语句，可以用逗号case多个值，可以通过fallthrough关键字继续尝试

内建函数
  append -- 追加slice
  len/cap -- 返回字符串、slice、数组等的长度/最大容量
  copy -- 复制slice, 并且返回赋值的元素的个数
  print/println -- 底层打印函数，可以在不引入fmt包的情况下使用，主要用于调试
  close/closed,,make,real,
  complex,real,imag -- 处理复数

利用多赋值(是这个名字吗？)，要交换两个变量的值非常简单，不需要定义中间变量
  x, y = y , x

定义包时，报名的约定是使用小写字符(不应有下划线或混合大小写)。 import [替代名] "包名"
  导入时(import)可以指定路径(如 "./mypackage")，并可选替代名(通常用于解决冲突)
  包中函数首字母大写时表示是导出的(包外部可见)，私有函数以小写字母开头

定时器：time.NewTimer(只通知一次)或time.NewTicker(持续通知)，可通过Stop()方法停止
  绑定在当前channel中，通过channel的阻塞通知机制来通知程序
  timer := time.NewTimer(2 * time.Second); <-timer.C; {定时满足}
  ticker := time.NewTicker(time.Second); for t := range ticker.C { xxxx }
计算耗时： t0 := time.Now(); xxx; elapsed:=time.Since(t0);
   或 t1:=time.Now(); elapsed:=t1.Sub(t0);
   计算时间段(可用于Sleep等)：time.Duration(nSec * time.Second)
随机数： rand.Seed(time.Now().Unix()); randNum:= rand.Intn(100); //0~99的随机数

*******************************************************************************/
package gostudy

import (
	"fmt"
	//"math"
	"testing"
	//"utf8"
)

//使用 iota 定义枚举
const (
	Monday = iota //默认值是0，其后的定义会自动递增，也可以手动指定值
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
	Sunday
)

func TestEnum(t *testing.T) {
	fmt.Printf("Sunday Number=%d\n", Sunday)
	if Sunday != 6 {
		t.Error("Enum 失败")
	}
}

func TestLanguage(t *testing.T) {
	var (
		iValue int = 100 //定义变量而不赋初始值，默认为其类型的null值(整数是0，字符串是零长度字符串)
	)
	iOtherValue := 100 //只可用在函数内的方式(变量类型由初始值推演出来)
	if iValue != iOtherValue {
		t.Error("定义变量")
	}

	//for 语句没有括号 -- 使用循环计算 1~100 的和
	var iSum int = 0
	for i := 1; i <= 100; i++ {
		iSum += i
	}
	fmt.Printf("iSum=%d\n", iSum)
	//t.Error(iSum == 5050)

	//数组
	str := "this is string" //字符串是常量
	arr := []byte(str)      //转换为字节数组
	arr[0] = 'T'
	fmt.Printf("str is \"%s\"\n", str)   //打印出来的仍是 ”this ..."
	str2 := string(arr)                  //创建新的字符串
	fmt.Printf("str2 is \"%s\"\n", str2) //打印出来的是 "This ..."

	//复数
	var com complex64 = 5 + 5i
	fmt.Printf("复数：5+5i = %v\n", com)
}

//数组切片(类似 Python) -- 英文是不是这个?
func TestArraySplit(t *testing.T) {

}

func TestLanguage2(t *testing.T) {
	t.Log("some info in language 2")
	//t.Errorf("some info in language 2")
}

func TestPointer(t *testing.T) {
	var intValue int = 10
	var pIntValue = &intValue
	(*pIntValue) += 15
	fmt.Printf("intValue=%d, pIntValue=%p, *pIntValue=%d\n", intValue, pIntValue, *pIntValue)

	var v []int = make([]int, 10, 100) // -- 分配一个整型数组，长度为10，容量为100，并返回前10个数组的切片
	for i := 0; i < len(v); i++ {
		v[i] = i
	}

}

func myMulti_ret(key string) (int, bool) {
	var err bool = false
	var val int = 0
	if key == "one" {
		val = 1
		err = true
	} else if key == "two" {
		val = 2
		err = true
	}
	return val, err
}

func TestMyMultiRet(t *testing.T) {
	if _, e := myMulti_ret("one"); e { // 注意 if 的分号后面有 e
		fmt.Printf("正常返回\n")
	} else {
		fmt.Printf("错误返回\n")
	}
}

func TestMySums(t *testing.T) {
	var iSum int = MySum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
	fmt.Printf("MySum for 1-10 number is %d\n", iSum)

	nums := []int{1, 2, 4}
	iSum = MySum(nums...) //在传递数组作为不定参数时，注意最后需要增加三个点
	fmt.Printf("MySum for 1-4 Array is %d\n", iSum)
}

func myFunReturnString(val int) string {
	return fmt.Sprintf("%d", val) //返回格式化的string
}

func TestRange(t *testing.T) {
	//从 array,slice,函数的不定参数 返回 int序号 + 值 的键值对
	list := []string{"a", "b", "c", "d", "e", "f"} //创建字符串的slice
	for idx, val := range list {
		fmt.Printf("Range from List[%d]=%s\n", idx, val)
	}

	strInfo := "Go语言测试"
	//从字符串string中返回 int序号 + 字符 的键值对
	//  注意：返回的pos是UTF8的字节位置，而非字符位置; char 是独立的Unicode字符
	//       即输出的 pos 为 { 0, 1, 2,5, 8, 11 } -- 术语为 rune
	for pos, char := range strInfo {
		fmt.Printf("Character '%c' at position %d\n", char, pos)
	}

	byteSilce := []byte(strInfo)
	fmt.Printf("byteSilce for strInfo is %v\n", byteSilce)

	//可以转换成[]int32, 每个int32保存Unicode编码
	intSilce := []int32(strInfo)
	fmt.Printf("intSlice for strInfo is %v\n", intSilce)

	//测试switch...case
	if UnHex('E') != 14 {
		t.Error("Switch case for UnHex Fail")
	}
}

func TestNumberConverter(t *testing.T) {
	var strTwo string = NumberConverter(2)
	if strTwo != "two" {
		t.Error("在map中存储和查找函数失败")
	}
}

func twiceFunc(num int) (result int) {
	result = num * 2
	return
}

func tripleFunc(num int) (result int) {
	result = num * 3
	return
}

//回调函数 -- 能否 typedef ?
func MYCallback(num int, f func(int) int) int {
	return f(num)
}

func TestCallbackFunc(t *testing.T) {
	//var funs [...]func(int)(int) = {   //这种语法为何编译不过?
	funs := [...]func(int) int{ //定义函数指针数组
		twiceFunc,
		tripleFunc,
	}
	for i := 0; i < len(funs); i++ {
		fmt.Printf("funs[%d]=%d\n", i, funs[i](10))
	}

	//传入函数指针，通过回调的方式进行调用
	numTwice := MYCallback(10, twiceFunc)
	fmt.Printf("twiceFun for 10 is %d\n", numTwice)
}
