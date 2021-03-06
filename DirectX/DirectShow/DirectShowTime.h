#ifndef DIRECT_SHOW_TIME_H
#define DIRECT_SHOW_TIME_H
#pragma once

//浅析DirectShow音视频同步解决方案 http://www.yesky.com/143/1805143.shtml
// Win8开发报名 http://msdn.microsoft.com/zh-cn/Hackathon 

//CBasePin::NewSegment -- 什么用处?
//  时间的最大(或初始值)一般可以设置为 MAX_TIME
/****************************************************************************************************
* 参考时钟(IReferenceClock) -- 所有的Filter采用统一的参考时钟，参考时钟保证了所有流的同步。
*   1.优先使用应用程序通过 IMediaFilter:: SetSyncSource 设置的参考时钟，如设置为NULL，表示以最快的速度处理Sample
*   2.支持 IReferenceClock 接口的 Live Source
*   3.从Render向上找实现了 IReferenceClock 的Filter，优先采用AudioRender(基于声卡硬件的时钟);
*   4.如果没有，则使用基于系统时间的缺省时钟( CLSID_SystemClock )
*     MS提供了 CSystemClock 类
* 
* DirectShow 中定义了两种时间：
*  1.参考时间(Reference Time, CRefTime 封装) -- 参考时钟返回的绝对时间(IReferenceClock::GetTime),
*    以100ns为单位( RefTime.h中定义了常量 UNITS = 10^7 )，利用价值不大
*    每帧的平均时间范围 VIDEOINFOHEADER::AvgTimePerFrame = UNITS / FrameRate , 如 10^7 / 25.00 = 400000 (100ns)
*  2.流时间(Stream Time) -- Sample上的时间戳(相对时间)。两次从参考时钟读取得数值的差，用于Graph的同步（如播放）。
*    即 IMediaSame::SetTime ? 通常第一个Sample起点为0,后面的时间戳根据Sample的有效长度和回放速率来确定。
*    也叫 Presentation time ? Renderer正是根据这个时间戳来控制播放，通常可以从 CBaseFilter::StreamTime 取得。
*    运行时：值 = 当前参考时钟时间 - 启动时的时间( IMediaFilter::Run 方法设置, 其值保存在 CBaseFilter::m_tStart )
*    暂停时：值 = 保持暂停那一刻的流时间
*    Seek后：值 = 复位为0
*    停止时：值 = 不确定
*  
*  IMediaSample的时间戳设置
*   1.推模式的Source Filter -- 需要对每一个Sample打时间戳(SetTime)
*     TODO:如果是实时的采集，一般不打时间戳(原因在SDK里有说明) ？ 
*   2.拉模式的Parser Filter -- 从文件中解析出时间信息(如AVI的索引表)，每一帧有时间戳
*
* 时间同步
*   1.流内同步 -- 保证单个媒体流内的时间关系，以满足感知要求，如按照规定的帧率播放一段视频
*   2.流间同步 -- 保证不同媒体流之间的时间关系，如音视频、字幕流之间的关系
*     音视频同步 -- latency (延时，mostly used for configuring audio video sync -- IAMLatency ) 
*       一个Filter的延时是这个Filter处理一个样本所需的时间总和，。在实时源中，延时取决于保存样本的缓冲区大小。
*         如 Graph 有一个33ms延时的视频源和一个具有500ms延时的音频源，则每个到达Renderer的视频帧要比与之匹配的音频样本到达音频renderer早470ms，
*            Graph 需要对这个差别进行补偿.
*       TODO?: AVI压缩时需要IConfigAviMux::MasterStream设置以哪个流(None[-1]、Video[0]、Audio[1])作为时间基准？ 
*              如果音频、视频采集不是同一个设备，采集一段时间后可能出现偏差，因此保存时需要以一个为基准，
*              调整另一个的帧率(一般是以音频为基准?)。
* 
*   传送到 Renderer Filter 的每个Sample都需要打时间戳(TimeStamp)，参考时钟以100ns为计时精度。
*   Video Renderer根据其TimeStamp来安排何时进行显示，或者丢弃一些sample。早了则等待，晚了则加快播放或丢弃。
*   但为了性能考虑(如实时抓取)，有时需要通过质量控制的方式，通知SourceFilter进行跳帧(丢包),
*   跳帧后为了不引起花屏现像，必须选择关键帧进行跳帧。
*   Seek之后Sample的时间戳会进行突变，流时间会有一个动态调整来确保Stream Time与新起始的Sample时间戳相吻合，
*   一般采取超前请求点Seek关键帧（这样得到的Samples对于Video Renderer而言是“迟到”的）
* 
* 时间信息
*   1.固定速率的媒体(固定帧率的视频或固定比特率的音频),将时间信息（帧率或比特率）置于文件首部，如 AVI::hdrl, MP4::moov box
*   2.变速率的媒体(MPEG TS和Real video),将时间信息嵌入媒体流的内部，而且可有效避免同步过程中的时间漂移
* 
* IMediaSample
*   SetTime(Presentation time) -- 设置时间戳(使用的是Stream Time)，用来同步音视频，表示Sample的有效时间范围(开始 -> 结束)。
*     不是所有的Sample都要求打时间戳(比如压缩数据)。
*     如果Render时Sample晚了或没有时间戳，会马上播放该Sample；如果来早了，会调用 IReferenceClock::AdviseTime 等待Sample的开始时间到达后再播放。
*     Sample上的时间戳一般由Source Filter或Parser Filter来设置
*     打时间戳的情况：
*       1.文件回放(Playback) -- 第一个Sample的时间戳从0开始打起，后面Sample的时间戳根据Sample有效数据的长度和回放速率来定;
*       2.音视频捕捉(Capture) -- 原则上，采集到的每一个Sample的开始时间都打上采集时刻的Stream time，
*                       视频采集，Preview Pin出来Sample不打时间戳，否则每个时间戳到Render都是晚了的(会因为质量控制照成丢帧)；
*                       音频采集，Audio Capture Filter与声卡驱动程序两者各自使用了不同的缓存，音频驱动以固定的时间间隔(如10ms)来填充采集filter的缓冲。
*                         在音频样本上的时间戳反映的是驱动填充采集filter时的时间，因此会有偏差，但媒体时间可以精确地反映缓冲区中音频样本的数量
*       3.合成(Mux) -- 取决于Mux后输出的数据类型，可以打时间戳，也可以不打时间戳（如 AVI文件格式使用固定的帧率而没有时间戳、
*   SetMediaTime -- 不是必须的，取决于实现。可打递增的序号，来判断是否有Sample丢失，依赖于程序实现。
*                   如 在视频流中，media time表示视频帧的数量。在音频流中，media time表示包中的样本数量。
* TIMECODE_SAMPLE(时间编码)
*   
*   
* 质量控制 -- 自适应的反馈机制，
*   IQualityControl -- 进行质量控制，用于通知数据流的速度是否合适，当播放的速度不对时，会查找到OutputPin上
*     Notify(IBaseFilter * pSender, Quality q);  
*       //q.Type -- Famine:速度太慢，Flood：速度太快；
*       //q.Proportion -- 请求调节的速度比率，以1000为基准：m_iRepeatTime = m_iRepeatTime*1000 / q.Proportion;
*       //q.Late -- 当前最新的Sample比其上的时间标记晚到了多少，负数表早到：m_rtSampleTime += q.Late;(直接跳过中间部分)
*       //q.TimeStamp -- 发送该消息时的时间，通常是Sample上的开始时间 -- 怎么用？
* 
* 
****************************************************************************************************/


#endif //DIRECT_SHOW_TIME_H