/*
 * Module:   TRTC 关键类型定义
 * 
 * Function: 分辨率、质量等级等枚举和常量值的定义
 *
 */

#ifndef __TRTCCLOUDDEF_H__
#define __TRTCCLOUDDEF_H__

#include <stdint.h>
#include <memory>

#ifdef LITEAV_EXPORTS
#define LITEAV_API __declspec(dllexport)
#else
#define LITEAV_API __declspec(dllimport)
#endif



/**
* 视频流类型
*/
enum TRTCVideoStreamType
{
    TRTCVideoStreamTypeBig = 0,                         ///< 大画面视频流
    TRTCVideoStreamTypeSmall = 1,                       ///< 小画面视频流
    TRTCVideoStreamTypeSub = 2,                         ///< 辅流（屏幕分享）
};

/**
* 设备事件类型
*/
enum TRTCDeviceState
{
    TRTCDeviceStateAdd = 0,                             ///< 添加设备
    TRTCDeviceStateRemove = 1,                          ///< 移除设备
    TRTCDeviceStateActive = 2,                          ///< 设备已启用
};

/**
* 视频画面填充模式
*/
enum TRTCVideoFillMode
{
    TRTCVideoFillMode_Fill = 0,                         ///< 图像铺满屏幕，超出显示视窗的视频部分将被截掉
    TRTCVideoFillMode_Fit = 1,                          ///< 图像长边填满屏幕，短边区域会被填充黑色
};

/**
 * Qos流控模式，本地控制还是云端控制
 */
enum TRTCQosMode
{
    TRTCQosModeClient,                                 ///< 客户端控制（用于SDK开发内部调试，客户勿用）
    TRTCQosModeServer,                                 ///< 云端控制（推荐线上使用）
};

/**
 * 对于画面质量偏好
 * 流畅 - 在遭遇弱网环境时，画面会变得模糊，且会有较多马赛克，但可以保持流畅不卡顿
 * 清晰 - 在遭遇弱网环境时，画面会尽可能保持清晰，但可能会更容易出现卡顿
 */
enum TRTCVideoQosPreference
{
    TRTCVideoQosPreferenceSmooth = 0,                  ///< 流畅
    TRTCVideoQosPreferenceClear = 1,                   ///< 清晰
};

/**
* 美颜风格
*/
enum TRTCBeautyStyle
{
    TRTCBeautyStyleSmooth = 0,                         ///< 光滑
    TRTCBeautyStyleNature = 1,                         ///< 自然
};


/////////////////////////////////////////////////////////////////////////////////
//
//                    【视频分辨率 TRTCVideoResolution】
//                   
//   此处仅有横屏分辨率，如果要使用 360x640 这样的竖屏分辨率，需要指定 ResolutionMode 为 Portrait
//
/////////////////////////////////////////////////////////////////////////////////

enum TRTCVideoResolution
{
	/// 宽高比1:1
    TRTCVideoResolution_120_120 = 1,     /// 建议码率 80kbps
    TRTCVideoResolution_160_160 = 3,     /// 建议码率 100kbps
    TRTCVideoResolution_270_270 = 5,     /// 建议码率 200kbps
    TRTCVideoResolution_480_480 = 7,     /// 建议码率 350kbps
    
    /// 宽高比4:3
    TRTCVideoResolution_160_120 = 50,    /// 建议码率 100kbps
    TRTCVideoResolution_240_180 = 52,    /// 建议码率 150kbps
    TRTCVideoResolution_280_210 = 54,    /// 建议码率 200kbps
    TRTCVideoResolution_320_240 = 56,    /// 建议码率 250kbps
    TRTCVideoResolution_400_300 = 58,    /// 建议码率 300kbps
    TRTCVideoResolution_480_360 = 60,    /// 建议码率 400kbps
    TRTCVideoResolution_640_480 = 62,    /// 建议码率 600kbps
    TRTCVideoResolution_960_720 = 64,    /// 建议码率 1000kbps
    
    /// 宽高比16:9
    TRTCVideoResolution_160_90  = 100,
    TRTCVideoResolution_256_144 = 102,
    TRTCVideoResolution_320_180 = 104,   /// 建议码率 250kbps
    TRTCVideoResolution_480_270 = 106,   /// 建议码率 350kbps
    TRTCVideoResolution_640_360 = 108,   /// 建议码率 550kbps
    TRTCVideoResolution_960_540 = 110,   /// 建议码率 850kbps
    TRTCVideoResolution_1280_720 = 112,  /// 建议码率 1200kbps
};

/**
* 编码模式
*/
enum TRTCVideoCodecMode
{
    TRTCVideoCodecModeSmooth = 0,       ///< 启用流畅编码模式，该模式下视频的弱网抗性和卡顿率明显好于兼容模式
    TRTCVideoCodecModeCompatible = 1,   ///< 强制启用兼容编码模式，支持绝大多数硬件编码器，性能优异，但卡顿率高于流畅编码模式
};

/**
* 设备类型
*/
enum TRTCDeviceType
{
    TRTCDeviceTypeUnknow = 0,
    TRTCDeviceTypeCamera,               ///< 摄像头
    TRTCDeviceTypeMic,                  ///< 麦克风
    TRTCDeviceTypeSpeaker,              ///< 扬声器
};

/**
* 视频帧的格式
*/
enum TRTCVideoPixelFormat
{
    TRTCVideoPixelFormat_Unknown = 0,
    TRTCVideoPixelFormat_I420 = 1,       ///< I420
    TRTCVideoPixelFormat_Texture_2D = 2, ///< OpenGL 2D 纹理
    TRTCVideoPixelFormat_BGRA32 = 3,     ///< BGRA32
};

/**
* 视频数据结构类型
*/
enum TRTCVideoBufferType
{
    TRTCVideoBufferType_Unknown = 0,
    TRTCVideoBufferType_Buffer = 1,     ///< 二进制Buffer类型
    TRTCVideoBufferType_Texture = 3,    ///< 纹理类型
};

/**
 * 视频画面旋转方向
 */
enum TRTCVideoRotation
{
    TRTCVideoRotation0   = 0,           ///< 顺时针旋转0度
    TRTCVideoRotation90  = 1,           ///< 顺时针旋转90度
    TRTCVideoRotation180 = 2,           ///< 顺时针旋转180度
    TRTCVideoRotation270 = 3,           ///< 顺时针旋转270度
};

/**
* 日志类型
*/
enum TRTCLogLevel
{
    TRTCLogLevelVerbos   = 0,           ///< 输出所有级别的log
    TRTCLogLevelDebug    = 1,           ///< 输出 DEBUG，INFO，WARNING，ERROR 和 FATAL 级别的log
    TRTCLogLevelInfo     = 2,           ///< 输出 INFO，WARNNING，ERROR 和 FATAL 级别的log
    TRTCLogLevelWarn     = 3,           ///< 只输出WARNNING，ERROR 和 FATAL 级别的log
    TRTCLogLevelError    = 4,           ///< 只输出ERROR 和 FATAL 级别的log
    TRTCLogLevelFatal    = 5,           ///< 只输出 FATAL 级别的log
    TRTCLogLevelNone     = 6,           ///< 不输出任何sdk log
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      【跨DLL访问接口类 TXString】
//                   
//   用于解决String在不同DLL之间传递时，由于运行时库版本不同而导致的编译和运行时不兼容问题
//
/////////////////////////////////////////////////////////////////////////////////

class TXStringImpl;
class LITEAV_API TXString
{
public:
    TXString();
    TXString(const char *s);
    TXString(const char *s, size_t size);
    TXString(const TXString& str);
    ~TXString();

    size_t size() const;
    bool empty() const;
    const char* c_str() const;

    TXString& operator =(const TXString& str);
    bool operator ==(const TXString& other) const;
    bool operator !=(const TXString& other) const;
    bool operator <(const TXString& other) const;
private:
    std::shared_ptr<TXStringImpl> m_impl;
};

class TXStringListImpl;
class LITEAV_API TXStringList
{
public:
    TXStringList();
    TXStringList(const TXStringList& list);
    ~TXStringList();

    size_t size() const;
    bool empty() const;

    void clear();
    void push_back(const TXString& str);
    void pop_back();
    TXString& operator [](size_t pos);

    TXStringList& operator =(const TXStringList& list);
private:
    std::shared_ptr<TXStringListImpl> m_impl;
};


/////////////////////////////////////////////////////////////////////////////////
//
//                      【进房参数 TRTCParams】
//                   
//   作为 TRTC SDK 的进房参数，只有该参数填写正确，才能顺利进入 roomId 制定的视频房间
//
/////////////////////////////////////////////////////////////////////////////////
struct TRTCParams
{
	/// 应用标识 - [必填] - 腾讯视频云基于 sdkAppId 完成计费统计
    uint32_t sdkAppId;
	/// 房间号码 - [必填] - 指定房间号，在同一个房间里的用户（userId）可以彼此看到对方并进行视频通话
    uint32_t roomId;
	/// 用户标识 - [必填] - 当前用户的 userid，相当于用户名
    TXString userId;
	/// 用户签名 - [必填] - 当前 userId 对应的验证签名，相当于登录密码
    TXString userSig;
	
	/// 房间签名 - [非必选] - 如果您希望某个房间（roomId）只让特定的某些用户（userId）才能进入，就需要使用 privateMapKey 进行权限保护
    TXString privateMapKey;
	/// 业务数据 - [非必选] - 某些非常用的高级特性才需要用到此字段
    TXString businessInfo;

    TRTCParams()
        : sdkAppId(0)
        , roomId(0)
        , userId("")
        , userSig("")
        , privateMapKey("")
        , businessInfo("")
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      【编码参数 TRTCVideoEncParam】
//
//   作为 TRTC SDK 的进房参数，只有该参数填写正确，才能顺利进入 roomId 制定的视频房间
//
/////////////////////////////////////////////////////////////////////////////////
struct TRTCVideoEncParam
{
	/// 视频分辨率
    ///
    /// @note 您在 TRTCVideoResolution 只能找到横屏模式的分辨率，比如： 640x360 这样的分辨率
    ///       如果想要使用竖屏分辨率，请指定 ResolutionMode 为 Portrait，比如：640x360 + Portrait = 360x640
    TRTCVideoResolution videoResolution;
	
	/// 编码器的编码模式（流畅 - 兼容）
    ///
    /// Smooth 模式（默认）：能够获得理论上最低的卡顿率，但性能略逊于 Compatible 模式
    /// Compatible 模式：使用最标准的视频编码模式，卡顿率高于 Smooth 模式，但性能优异，推荐在低端设备上开启
    TRTCVideoCodecMode codecMode;
	
	/// 视频采集帧率，推荐设置为 15fps 或 20fps，10fps以下会有明显的卡顿感，20fps以上则没有必要
    ///
    /// @note 很多廉价的 USB 摄像头可能并不支持 15fps 以上的采集帧率
    ///
    uint32_t videoFps;

	/// 视频上行码率
	///
    /// @note 推荐设置请参考 TRTCVideoResolution 定义处的注释说明	
    uint32_t videoBitrate;

    TRTCVideoEncParam()
        : videoFps(15)
        , codecMode(TRTCVideoCodecModeSmooth)
        , videoResolution(TRTCVideoResolution_640_360)
        , videoBitrate(500)
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      【音量大小 TRTCVolumeInfo】
//
//  表示语音音量的评估大小，通过这个数值，您可以在 UI 界面上用图标表征 userId 是否有在说话 
//
/////////////////////////////////////////////////////////////////////////////////
struct TRTCVolumeInfo
{
    TXString userId;                        ///< 说话者的userId
    uint32_t volume;                        ///< 说话者的音量, 取值范围 0~100

    TRTCVolumeInfo()
        : userId("")
        , volume(0)
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      【质量评分 TRTCQualityInfo】
//                   
//   质量评分，通过这个数值，您可以在 UI 界面上用图标表征 userId 的通话线路质量
//
/////////////////////////////////////////////////////////////////////////////////

enum TRTCQuality
{
    TRTCQuality_Unknown       = 0,
    TRTCQuality_Excellent     = 1,
    TRTCQuality_Good          = 2,
    TRTCQuality_Poor          = 3,
    TRTCQuality_Bad           = 4,
    TRTCQuality_Vbad          = 5,
    TRTCQuality_Down          = 6, 
};

struct TRTCQualityInfo
{
    TXString userId;                        ///< 用户标识
    TRTCQuality quality;                    ///< 视频质量

    TRTCQualityInfo()
        : userId("")
        , quality(TRTCQuality_Unknown)
    {

    }
};

/**
* 视频帧数据
*/
struct TRTCVideoFrame
{
    TRTCVideoPixelFormat videoFormat;       ///< 视频帧的格式
    TRTCVideoBufferType bufferType;         ///< 视频数据结构类型
    char* data;                             ///< 视频数据，字段bufferType是TRTCVideoBufferType_Buffer时生效
    int textureId;                          ///< 视频纹理ID，字段bufferType是TRTCVideoBufferType_Texture时生效
    uint32_t length;                        ///< 视频数据的长度，单位是字节，对于i420而言， length = width * height * 3 / 2，对于BGRA32而言， length = width * height * 4
    uint32_t width;                         ///< 画面的宽度
    uint32_t height;                        ///< 画面的高度
    uint64_t timestamp;                     ///< 时间戳，单位ms
    TRTCVideoRotation rotation;             ///< 画面旋转角度

    TRTCVideoFrame()
        : videoFormat(TRTCVideoPixelFormat_Unknown)
        , data(NULL)
        , textureId(-1)
        , length(0)
        , width(640)
        , height(360)
        , timestamp(0)
        , rotation(TRTCVideoRotation0)
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////
//
//                      【网络测速结果 TRTCSpeedTestResult】
//
//  您可以在用户进入房间前通过 TRTCCloud 的 startSpeedTest 接口进行测速 （注意：请不要在通话中调用）
//  测速结果会每 2~3 秒钟返回一次，每次返回一个 ip 地址的测试结果，其中：
//  
//  >> quality 是内部通过评估算法测算出的网络质量，loss 越低，rtt 越小，得分也就越高
//  >> upLostRate 是指上行丢包率，例如 0.3 代表每向服务器发送10个数据包，可能有3个会在中途丢失
//  >> downLostRate 是指下行丢包率，例如 0.2 代表从服务器每收取10个数据包，可能有2个会在中途丢失
//  >> rtt 是指当前设备到腾讯云服务器的一次网络往返时间，正常数值在 10ms ~ 100ms 之间
//  
/////////////////////////////////////////////////////////////////////////////////

struct TRTCSpeedTestResult
{
    /// 服务器ip地址
    TXString ip;

    /// 网络质量
    ///
    /// 内部通过评估算法测算出的网络质量，loss 越低，rtt 越小，得分也就越高
    TRTCQuality quality;

    /// 上行丢包率，范围是[0,1.0]
    ///
    /// 例如 0.3 代表每向服务器发送10个数据包，可能有3个会在中途丢失
    float upLostRate;

    /// 下行丢包率，范围是[0,1.0]
    ///
    /// 例如 0.2 代表从服务器每收取10个数据包，可能有2个会在中途丢失
    float downLostRate;

    /// 延迟（毫秒）：代表 SDK 跟服务器一来一回之间所消耗的时间，这个值越小越好
    ///
    /// 是指当前设备到腾讯云服务器的一次网络往返时间，正常数值在 10ms ~ 100ms 之间
    int rtt;

    TRTCSpeedTestResult()
        : ip("")
        , upLostRate(0.0f)
        , downLostRate(0.0f)
        , rtt(0)
    {

    }
};


#endif /* __TRTCCLOUDDEF_H__ */
