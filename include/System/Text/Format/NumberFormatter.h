#pragma once

#include <Meta/Array.h>
#include <System/Buffer.h>
#include <System/IO/Scan.h>
#include <System/IO/TextWriter.h>
#include <System/Text/Encoding.UTF8.h>
#include <System/Text/Format/Formatter.h>
#include <TypeTraits.h>
#include <Types.h>

namespace System::Text::Format {
    using IO::ITextWriter;
    using IO::Scan;

    struct NumberFormatter
    {
        bool        UseAutoWidth     = false;
        bool        UseAutoPrecision = false;
        bool        UseCapitalized   = false;
        bool        UsePrefix        = false;
        char const* Prefix           = "";
        usize       Base             = 10;
        usize       Width            = 0;
        usize       Precision        = 0;
        // TODO: Allow different fills for different locales or encodings
        char        Fill             = ' ';

        void Parse(Scan<UTF8>& scan)
        {
            if (scan.SkipOnce('#')) {
                UsePrefix = true;

                Rune c = scan.Next();
                if (Traits<char>::isUpperCase(c)) {
                    c              = Traits<char>::toLowerCase(c);
                    UseCapitalized = true;
                }

                switch (c) {
                    case 'b': Base = 2; break;
                    case 'o': Base = 8; break;
                    case 'd': Base = 10; break;

                    case 'x': {
                        Base         = 16;
                        Fill         = '0';
                        Width        = sizeof(usize) * 2;
                        UseAutoWidth = true;
                        break;
                    }

                    case 'u': {
                        Base         = 16;
                        Fill         = '0';
                        UseAutoWidth = true;
                        UsePrefix    = false;
                        break;
                    };

                    default: break;
                }
            }
        }

        template <typename T>
            requires Std::isIntegral<T> or Std::isFloatingPoint<T>
        usize Format(ITextWriter auto& writer, T val)
        {
            usize len = 0;
            if constexpr (Std::isSigned<T>) {
                if (val < 0) {
                    writer('-');
                    ++len;
                    val = -val;
                }
            }

            auto digit = [this](usize v) {
                if (v < 10) {
                    return '0' + v;
                }
                return (UseCapitalized ? 'A' : 'a') + (v - 10);
            };
            Buffer<char, 64> buf;

            do {
                buf += digit(val % Base);
                val /= Base;
            } while (val != 0 and buf.Length < buf.Capacity);

            if (UseAutoWidth)
                Width = sizeof(T) * 2;

            while (Width > buf.Length) {
                buf += Fill;
            }
            Arrays::Reverse(buf.Data, buf.Length);

            if (UsePrefix) {
                // TODO: Use a better way to handle prefixes
                writer(Prefix[0]);
                writer(Prefix[1]);
                len += 2;
            }

            for (usize i = 0; i < buf.Length; i++) {
                writer(buf[i]);
                ++len;
            }

            return len;
        };
    };

    template <typename T>
        requires Std::isIntegral<T> or Std::isFloatingPoint<T>
    struct Formatter<T> : public NumberFormatter
    {};
}