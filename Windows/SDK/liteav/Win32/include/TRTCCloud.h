/*
 * Module:   TRTCCloud @ TXLiteAVSDK
 * 
 * Function: 腾讯云视频通话功能的主要接口类
 *
 */
 
#ifndef __TRTCENGINE_H__
#define __TRTCENGINE_H__

#include "TRTCCloudCallback.h"
#include "TRTCCloudDef.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

class TRTCCloudImpl;

class LITEAV_API TRTCCloud
{
public:
    TRTCCloud();
    ~TRTCCloud();

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                       设置 TRTCCloudCallback 回调
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 设置 TRTCCloudCallback 回调
    /// @{
    /**
    * \brief 添加事件回调
    * 
    * \param callback 事件回调
    */
    void addCallback(ITRTCCloudCallback* callback);

    /**
    * \brief 移除事件回调
    *
    * \param callback 事件回调
    */
    void removeCallback(ITRTCCloudCallback* callback);
    /// @}
    
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （一）房间相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 房间相关接口函数
    /// @{
    /**
    * \brief 1.1 进入房间
    *
    * \param params 进房参数，详情参考TRTCParams定义。
	* \note 不管进房是否成功，都必须与exitRoom配对使用，在调用 exitRoom 前再次调用 enterRoom 函数会导致不可预期的错误问题
    */
    void enterRoom(const TRTCParams& params);

    /**
    * \brief 1.2 退出房间
    */
    void exitRoom();
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （二）视频相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name  视频相关接口函数
    /// @{
    /**
    * \brief 2.1 启动本地摄像头采集和预览
    *
    * \param rendHwnd 承载预览画面的 HWND
    * \attention 这个接口会启动默认的摄像头，可以通过 setCurrentCameraDevice 接口选用其他摄像头
    */
    void startLocalPreview(HWND rendHwnd);

    /**
    * \brief 2.2 关闭本地摄像头采集和预览
    */
    void stopLocalPreview();

    /**
    * \brief 2.3 开始渲染远端用户画面
    *
    * \param userId   远端用户标识
    * \param rendHwnd 承载预览画面的 HWND
    */
    void startRemoteView(const char* userId, HWND rendHwnd);

    /**
    * \brief 2.4 停止渲染远端用户画面
    *
    * \param userId 远端用户标识
    */
    void stopRemoteView(const char* userId);

    /**
    * \brief 2.5 停止渲染所有远端用户画面，如果有屏幕分享，则屏幕分享的画面也会一并被关闭
    */
    void stopAllRemoteView();

    /**
    * \brief 2.6 设置画面质量参数
    *
    * \param params        视频编码参数，详情请参考 TRTCCloudDef.h 中的 TRTCVideoEncParam 定义
    * \param qosMode       流控模式选择，默认选择【云控】模式，便于获得更好的效果，【终端】模式则用于特殊的调试场景
    * \param qosPreference 画面质量偏好，有【流畅】和【清晰】两种模式可供选择，详情请参考 TRTCVideoQosPreference 的定义
    */
    void setLocalVideoQuality(const TRTCVideoEncParam& params, TRTCQosMode qosMode, TRTCVideoQosPreference qosPreference);

    /**
    * \brief 2.7 是否屏蔽本地视频
    *
    * 当屏幕本地视频后，房间里的其它成员会收到 onUserVideoAvailable 回调通知
    * \param mute true: 屏蔽视频采集和上行，false: 开启视频采集和上行
    */
    void muteLocalVideo(bool mute);

    /**
    * \brief 2.8 设置本地图像的渲染模式
    *
    * \param mode 填充（画面可能会被拉伸裁剪）还是适应（画面可能会有黑边）
    */
    void setLocalViewFillMode(TRTCVideoFillMode mode);

    /**
    * \brief 2.9 设置远端图像的渲染模式
    *
    * \param userId 远端用户标识
    * \param mode 填充（画面可能会被拉伸裁剪）还是适应（画面可能会有黑边）
    */
    void setRemoteViewFillMode(const char* userId, TRTCVideoFillMode mode);

    /**
    * \brief 2.10 设置本地图像的顺时针旋转角度
    *
    * \param rotation 支持 90、180、270 旋转角度
    */
    void setLocalViewRotation(TRTCVideoRotation rotation);

    /**
    * \brief 2.11 设置远端图像的顺时针旋转角度
    *
    * \param userId 远端用户标识
    * \param rotation 支持 90、180、270 旋转角度
    */
    void setRemoteViewRotation(const char* userId, TRTCVideoRotation rotation);

    /**
    * \brief 2.12 设置视频编码输出的（也就是远端用户观看到的，以及服务器录制下来的）画面方向
	*
    * \param rotation 支持 90、180、270 旋转角度
    */
    void setVideoOutputRotation(TRTCVideoRotation rotation);

    /**
    * \brief 2.13 开启大小画面双路编码模式
    *
    *  如果当前用户是房间中的主要角色（比如主播、老师、主持人...），并且使用 PC 或者 Mac 环境，可以开启该模式
    *  开启该模式后，当前用户会同时输出【高清】和【低清】两路视频流（但只有一路音频流）
    *  对于开启该模式的当前用户，会占用更多的网络带宽，并且会更加消耗 CPU 计算资源
    *  对于同一房间的远程观众而言，
    *  如果有些人的下行网络很好，可以选择观看【高清】画面
    *  如果有些人的下行网络不好，可以选择观看【低清】画面
    * \param enable 是否开启小画面编码
    * \param smallVideoParam 小流的视频参数，必须和 setLocalVideoQuality 接口的params参数具有相同的宽高比，也就是分辨率宽度和高度比值相同
    */
    void enableSmallVideoStream(bool enable, const TRTCVideoEncParam& smallVideoParam);

    /**
    * \brief 2.14 选择某一路的画面质量：当网络不好时可以切换到低清晰度的小画面
    *
    *      如果对方没有开启双路视频模式，则此操作无效
    * \param userId 远端用户标识
    * \param type 大小流类型
    */
    void setRemoteVideoStreamType(const char* userId, TRTCVideoStreamType type);

    /**
    * \brief 2.15 设定观看方优先选择的视频质量
    *
    *  低端设备推荐优先选择低清晰度的小画面
    *  如果对方没有开启双路视频模式，则此操作无效
    * \param type 默认大小流类型
    * \attention 默认优先选择大画面
    */
    void setPriorRemoteVideoStreamType(TRTCVideoStreamType type);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （三）音频相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 音频相关接口函数
    /// @{
    /**
    * \brief 3.1 是否屏蔽本地音频
    *
    * \note 当屏蔽本地音频后，房间里的其它成员会收到 onUserAudioAvailable 回调通知
    * \param mute true: 关闭音频采集和上行 false: 开启音频采集以及音频上行
    */
    void muteLocalAudio(bool mute);

    /**
    * \brief 3.2 屏蔽指定远端音频
    *
    * \param userId 用户id
    * \param mute 开关
    */
    void muteRemoteAudio(const char* userId, bool mute);

    /**
    * \brief 3.3 远端所有用户全部静音
    *
    * \param mute 开关
    */
    void muteAllRemoteAudio(bool mute);

    /**
    * \brief 3.4 启用或关闭音量大小提示
    *
    *  开启后会在 onUserVoiceVolume 中获取到 SDK 对音量大小值的评估
    * \param interval 建议设置为大于 200 毫秒，最小不小于 20 毫秒, 设置为 0 表示关闭
    * \param smoothLevel  灵敏度，[0,10], 数字越大，波动越灵敏
    */
    void enableAudioVolumeEvaluation(uint32_t interval, uint32_t smoothLevel);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （四）摄像头相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 摄像头相关接口函数
    /// @{
    /**
    * \brief 4.1 查询摄像头列表
    *
    * \return 摄像头列表，字符编码格式是UTF-8
    */
    TXStringList getCameraDevicesList();

    /**
    * \brief 4.2 设置要使用的摄像头
    *
    * \param deviceId 摄像头ID，getCameraDevicesList 接口获取得到
    */
    void setCurrentCameraDevice(const char* deviceId);

    /**
    * \brief 4.2 获取当前使用的摄像头
    *
    * \return 摄像头名称，字符编码格式是UTF-8
    */
    TXString getCurrentCameraDevice();
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （五）音频设备相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 音频设备相关接口函数
    /// @{
    /**
    * \brief 5.1 查询麦克风列表
    *
    * \return 麦克风列表，字符编码格式是UTF-8
    */
    TXStringList getMicDevicesList() const;

    /**
    * \brief 5.2 选择指定的麦克风作为录音设备，不调用该接口时，默认选择索引为0的麦克风
    *
    * \param micId 麦克风Id，getMicDevicesList 接口查询获取
    */
    void setCurrentMicDevice(const char* micId);

    /**
    * \brief 5.3 获取当前选择的麦克风
    *
    * \return 麦克风名称，字符编码格式是UTF-8
    */
    TXString getCurrentMicDevice();

    /**
    * \brief 5.4 查询已选择麦克风的音量
    *
    * \return 音量值，范围是[0, 100]
    */
    uint32_t getCurrentMicDeviceVolume();

    /**
    * \brief 5.5 设置已选择麦克风的音量
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    void setCurrentMicDeviceVolume(uint32_t volume);

    /**
    * \brief 5.6 查询扬声器列表
    *
    * \return 扬声器列表，字符编码格式是UTF-8
    */
    TXStringList getSpeakerDevicesList() const;

    /**
    * \brief 5.7 选择指定的扬声器作为音频播放的设备，不调用该接口时，默认选择索引为0的扬声器
    *
    * \param speakerId 麦克风Id，getSpeakerDevicesList 接口查询获取
    */
    void setCurrentSpeakerDevice(const char* speakerId);

    /**
    * \brief 5.8 获取已选择的扬声器
    *
    * \return 扬声器名称，字符编码格式是UTF-8
    */
    TXString getCurrentSpeakerDevice();

    /**
    * \brief 5.9 查询已选择扬声器的音量，注意查询得到不是系统扬声器的音量大小
    *
    * \return 音量值，范围是[0, 100]
    */
    uint32_t getCurrentSpeakerVolume();

    /**
    * \brief 5.10 设置SDK播放的音量，注意设置的不是系统扬声器的音量大小
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    void setCurrentSpeakerVolume(uint32_t volume);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （六）美颜相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 美颜相关接口函数
    /// @{
    /**
    * \brief 6.1 设置美颜、美白、红润
    *
    * \param style     美颜风格
    * \param beauty    美颜级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显
    * \param white     美白级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显
    * \param ruddiness 红润级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显，该参数暂未生效
    */
    void setBeautyStyle(TRTCBeautyStyle style, uint32_t beauty, uint32_t white, uint32_t ruddiness);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （七）屏幕共享接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 屏幕采集共享操作
    /// @{
    /**
    * \brief 7.1 启动屏幕分享
    *
    * \param rendHwnd - 承载预览画面的 HWND
    * \param rendRect - 指定视频图像在 HWND 上的渲染区域
    * \param captureHwnd - 指定分享源
    * \param captureRect - 指定捕获的区域
    * \param captureGLOrDXWindow - 无法直接获取某些特殊窗口（openGL渲染的窗口）的画面，captureGLOrDXWindow 设为 true 时，通过截取屏幕区域
    * 实现捕获窗口，默认设为 false
    * 共享整个屏幕 : captureHwnd 设为 NULL，captureRect 设为 { 0, 0, 0, 0 }。
    * 共享指定窗口 : captureHwnd 设为非 NULL，captureRect 设为需要的区域
    * 共享指定区域 : captureHwnd 设为 NULL，captureRect 设为非 NULL
    */
    void startScreenCapture(HWND rendHwnd, const RECT& rendRect, HWND captureHwnd, const RECT& captureRect, bool captureGLOrDXWindow);

    /**
    * \brief 7.2 更新采集区域
    *
    * \param captureRect 指定捕获的区域
    */
    void resetScreenCaptureRect(const RECT& captureRect);

    /**
    * \brief 7.3 关闭屏幕分享
    */
    void stopScreenCapture();
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （八）音视频自定义接口
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 音视频自定义接口
    /// @{

    /**
    * \brief 8.1 启用视频自定义采集模式，即放弃SDK原来的视频采集流程，改用sendCustomVideoData向SDK塞入自己采集的视频画面
    *
    * \param enable 是否启用
    */
    void enableCustomVideoCapture(bool enable);

    /**
    * \brief 8.2 发送客户自采集的视频数据
	* 内部有简单的帧率调控逻辑，如果该方法被调用得太频繁，SDK会自动丢弃一些视频帧；如果该方法被调用得太慢，SDK会自动补充一些重复的画面
    *
    * \param userId 用户标识
    * \param frame 视频帧数据
    */
    void sendCustomVideoData(const char* userId, TRTCVideoFrame* frame);

    /**
    * \brief 8.3 启用音频自定义采集模式，即放弃SDK原来的声音采集流程，改用enableCustomAudioCapture向SDK塞入自己采集的声音数据（PCM格式）
    *
    * \param enable 是否启用
    */
    void enableCustomAudioCapture(bool enable);

    /**
    * \brief 8.4 发送客户自定义的音频PCM数据
    *
    * 如果是单声道，请保证每次传入的PCM长度为2048个采样点；如果是双声道，请保证每次传入的PCM长度为4096个采样点
	* 要求每个采样点的位宽是 16bit
	*
    * \param pcmBuffer pcm音频数据
    */
    void sendCustomAudioData(const char* pcmBuffer);

	 /**
    * \brief 8.5 设置本地视频自定义渲染
    *
    * \note              设置此方法，SDK内部会把采集到的数据回调出来，SDK跳过HWND渲染逻辑
                         退房、setLocalVideoRenderCallback(null, x)，停止回调
    * \param callback    自定义渲染回调
    * \param pixelFormat 指定回调的像素格式
    * \return 0：成功， <0：错误
    */
    int setLocalVideoRenderCallback(ITRTCVideoRenderCallback* callback, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType);

    /**
    * \brief 8.6 设置远端视频自定义渲染
    *
    * \note              设置此方法，SDK内部会把远端的数据解码后回调出来，SDK跳过HWND渲染逻辑
                         退房、setLocalVideoRenderCallback(null, x)、远端用户退房，停止回调
    * \param userId      用户标识
    * \param callback    自定义渲染回调
    * \param pixelFormat 指定回调的像素格式
    * \return 0：成功， <0：错误
    */
    int setRemoteVideoRenderCallback(const char* userId, ITRTCVideoRenderCallback* callback, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType);
	
	/**
	* \brief 8.8 设置本地视频的二次加工回调
	*
	* \note 设置之后，SDK会在送编码器之前将该帧视频画面回调出来，供您进行二次图像加工（比如叠加字幕，角标等等），之后SDK会将处理后的画面送入编码器
	* \note 注意：由于该回调过程是同步的，所以请注意控制二次加工的时间，不宜超过 20ms，否则会导致画面卡顿
	* \note setLocalVideoPreprocessCallback(null)可以停止回调
	*
	* \param callback    自定义渲染回调
	*/
	void setLocalVideoPreprocessCallback(ITRTCVideoPreprocessCallback* callback);
	
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （九）自定义消息发送
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 自定义消息发送
    /// @{
    /**
    * \brief 9.1 发送自定义消息给房间内所有用户
    *
    * \param cmdId    消息ID，取值范围为 1 ~ 10
    * \param data     待发送的数据，最大支持 1KB（1000字节）的数据大小
    * \param dataSize 待发送的数据大小
    * \param reliable 是否可靠发送，可靠发送的代价是会引入一定的延时，因为接收端要暂存一段时间的数据来等待重传
    * \param ordered  是否要求有序，即是否要求接收端接收的数据顺序和发送端发送的顺序一致，这会带来一定的接收延时，因为在接收端需要暂存并排序这些消息
    * \return true：消息已经发出 false：消息发送失败
	*
    * @note 限制1：发送消息到房间内所有用户，每秒最多能发送 30 条消息
    *       限制2：每个包最大为 1 KB，超过则很有可能会被中间路由器或者服务器丢弃
    *       限制3：每个客户端每秒最多能发送总计 8 KB 数据
    *
    *       请将 reliable 和 ordered 同时设置为 YES 或 NO, 暂不支持交叉设置。
    *       有序性（ordered）是指相同 cmdID 的消息流一定跟发送方的发送顺序相同，
    *       强烈建议不同类型的消息使用不同的 cmdID，这样可以在要求有序的情况下减小消息时延
    */
    bool sendCustomCmdMsg(uint32_t cmdId, const uint8_t* data, uint32_t dataSize, bool reliable, bool ordered);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十）背景混音相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 背景混音相关接口函数
    /// @{
    /**
    * \brief 10.1 播放背景音乐
    *
    * \param path 音乐文件路径
    */
    void playBGM(const char* path);

    /**
    * \brief 10.2 停止播放背景音乐
    */
    void stopBGM();

    /**
    * \brief 10.3 暂停播放背景音乐
    */
    void pauseBGM();

    /**
    * \brief 10.4 继续播放背景音乐
    */
    void resumeBGM();

    /**
    * \brief 10.5 获取音乐文件总时长，单位毫秒
    *
    * \param path 音乐文件路径，如果path为空，那么返回当前正在播放的music时长
    * \return     返回音频文件的时长，单位ms。
    */
    uint32_t getBGMDuration(const char* path);

    /**
    * \brief 10.6 设置BGM播放进度
    *
    * \param pos 单位毫秒
    */
    void setBGMPosition(uint32_t pos);

    /**
    * \brief 10.7 设置麦克风的音量大小，播放背景音乐混音时使用，用来控制麦克风音量大小
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    void setMicVolumeOnMixing(uint32_t volume);

    /**
    * \brief 10.8 设置背景音乐的音量大小，播放背景音乐混音时使用，用来控制背景音音量大小
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    void setBGMVolume(uint32_t volume);
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十一）设备和网络测试
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 设备和网络测试
    /// @{
    /**
    * \brief 11.1 开始进行网络测速(视频通话期间请勿测试，以免影响通话质量)
	*
	* 测速结果将会用于优化 SDK 接下来的服务器选择策略，因此推荐您在用户首次通话前先进行一次测速，这将有助于我们最佳的服务器
    * 同时，如果测试结果非常不理想，您可以通过醒目的 UI 提示用户选择更好的网络
    * 
    * 注意：测速本身会消耗一定的流量，所以也会产生少量额外的流量费用
	*
    * \param sdkAppId 应用标识
    * \param userId 用户标识
    * \param userSig 用户签名
    */
    void startSpeedTest(uint32_t sdkAppId, const char* userId, const char* userSig);

    /**
    * \brief 11.2 停止服务器测速
    */
    void stopSpeedTest();

    /**
    * \brief 11.3 开启摄像头测速，会触发 onLocalVideoFrameAfterProcess 回调接口
    *
    * \param rendHwnd 承载预览画面的 HWND
    * \attention 这个接口会启动默认的摄像头，可以通过 setCurrentCameraDevice 接口指定其他摄像头
    */
    void startCameraDeviceTest(HWND rendHwnd);

    /**
    * \brief 11.4 停止摄像头测速
    */
    void stopCameraDeviceTest();

    /**
    * \brief 11.5 开启麦克风测试，回调接口 onTestMicVolume 获取视频数据
    *
    * \param interval 反馈音量提示的时间间隔（ms），建议设置到大于 200 毫秒
    */
    void startMicDeviceTest(uint32_t interval);

    /**
    * \brief 11.6 关闭麦克风测试
    */
    void stopMicDeviceTest();

    /**
    * \brief 11.7 开启扬声器测试，回调接口 onTestSpeakerVolume 获取视频数据
    *
    * 该方法测试扬声器是否能正常工作。SDK播放指定的音频文件，测试者如果能听到声音，说明播放设备能正常工作
    * \param testAudioFilePath 音频文件的绝对路径，路径字符串使用 UTF-8 编码格式，支持文件格式: wav、mp3
    */
    void startSpeakerDeviceTest(const char* testAudioFilePath);

    /**
    * \brief 11.8 停止扬声器测试
    */
    void stopSpeakerDeviceTest();
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十二）LOG相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 调试相关函数
    /// @{
    /**
    * \brief 12.1 获取SDK版本信息
    */
    static TXString getSDKVersion();

    /**
    * \brief 12.2 设置log输出级别
    *
    * \param level 参见 TRTCLogLevel
    */
    static void setLogLevel(TRTCLogLevel level);

    /**
    * \brief 12.3 启用或禁用控制台日志打印
    *
    * \param enabled 指定是否启用
    */
    static void setConsoleEnabled(bool enabled);

    /**
    * \brief 12.4 启用或禁用Log的本地压缩
    *
    *  开启压缩后，log存储体积明显减小，但需要腾讯云提供的 python 脚本解压后才能阅读
    *  禁用压缩后，log采用明文存储，可以直接用记事本打开阅读，但占用空间较大。
    * \param enabled 指定是否启用
    */
    static void setLogCompressEnabled(bool enabled);

    /**
    * \brief 12.5 设置日志保存路径
    *
    * \param path 存储日志的文件夹，例如 "D:\\Log"，utf-8编码
    * \attention 如果没有调用这个接口，SDK 会将日志输出到默认的位置，位于 C:/Users/[系统用户名]/AppData/Roaming/Tencent/liteav/，即 %appdata%/Tencent/liteav
    */
    static void setLogDirPath(const char* path);

    /**
    * \brief 12.6 设置日志回调
    *
    * \param callback 日志回调
    */
    static void setLogCallback(ITRTCLogCallback* callback);

    /**
    * \brief 12.7 显示仪表盘（状态统计和事件消息浮层view），方便调试
    *
    * \param showType 0: 不显示 1: 显示精简版 2: 显示全量版
    */
    static void showDebugView(int showType);
    /// @}
private:
    static TRTCCloud* m_instance;
    std::shared_ptr<TRTCCloudImpl> m_impl;
};

#endif /* __TRTCENGINE_H__ */
