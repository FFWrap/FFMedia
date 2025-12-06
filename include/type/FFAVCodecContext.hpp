#pragma once

#include "type/impl/ffavimpl.hpp"
#include "error/ffav.hpp"

#include <memory>
#include <vector>

namespace ff {
    class FFAVCodecContextHWFormat;
    using FFAVCodecContextHWFormatPtr = std::shared_ptr<FFAVCodecContextHWFormat>;

    class FFAVCodecContext;
    typedef std::shared_ptr<FFAVCodecContext> FFAVCodecContextPtr;

    class FFAVCodecContext {
    public:
        explicit FFAVCodecContext();
        virtual ~FFAVCodecContext();

    public:
        static FFAVCodecContextPtr create();

    public:
        AVError openCodec();

    public:  // get set
        FFAVCodecContextImplPtr getImpl();

        void setPixFmt(int format);
        void setCodecName(const std::string& codecName);
        void setHWFormat(FFAVCodecContextHWFormatPtr hwFormat);
        AVError setOpt(const std::string& key, const std::string& value);

        std::string getCodecName();
        int getPixFmt();
        FFAVCodecContextHWFormatPtr getHWFormat();

        bool findHWFormat();

        bool isHWCodec();
        bool isCodecOpen();

    private:
        int pixFmt; // ffmpeg pix_fmt
        FFAVCodecContextHWFormatPtr hwFormat;

        bool isCodecOpenFlag;
        std::string codecName;

        FFAVCodecContextImplPtr codecContextImpl;
    };

    using FFAVCodecContextList = std::vector<FFAVCodecContextPtr>;
}
