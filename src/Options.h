//
// Created by engin on 5.11.2017.
//

#ifndef LIMONENGINE_OPTIONS_H
#define LIMONENGINE_OPTIONS_H

#include <glm/glm.hpp>
#include <iostream>
#include <tinyxml2.h>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Utils/HashUtil.h"
#include "Utils/Logger.h"
#include "API/LimonTypes.h"

namespace OptionsUtil {
    class Options {
    public:
        static constexpr float PI = 3.14159265358979f;
        static constexpr float PI_DOUBLE = 3.141592653589793238463;
        enum class MoveModes {
            WALK, RUN
        };
        enum class TextureFilteringModes {
            NEAREST, BILINEAR, TRILINEAR
        };

        auto findOption(uint64_t optionHash) const {
            std::unordered_map<std::string, std::shared_ptr<LimonTypes::GenericParameter>>::const_iterator it = this->options.begin();
            for(; it != this->options.end(); it++) {
                if (HashUtil::hashString(it->first) == optionHash) {
                    return it;
                }
            }
            return it;
        };


        template<typename T>
        class Option {
            std::shared_ptr<LimonTypes::GenericParameter> value;
            bool isSet = false;
            std::vector<long>* longValues = nullptr;
            explicit Option(const std::shared_ptr<LimonTypes::GenericParameter> &value, bool isSet) : value(value), isSet(isSet) {
                if(value->valueType == LimonTypes::GenericParameter::LONG_ARRAY) {
                    longValues = new std::vector<long>();
                    longValues->reserve(value->value.longValues[0]);
                }
            }

            friend class Options;

        public:
            bool isUsable() const { return isSet; };

            template<typename Q =  T, typename std::enable_if<!std::is_same<Q, std::string>::value>::type* = nullptr, typename std::enable_if<!std::is_same<Q, std::vector<long>>::value>::type* = nullptr>
            T get() const {
                if (!isSet) {
                    std::cerr << "Option " << value->description << " is not set" << std::endl;
                }
                return *((T *) &(value->value));
            };

            template<typename Q =  T, typename std::enable_if<!std::is_same<Q, std::string>::value>::type* = nullptr, typename std::enable_if<!std::is_same<Q, std::vector<long>>::value>::type* = nullptr>
            T getOrDefault(T defaultValue) const {
                if (!isSet) {
                    return defaultValue;
                }
                return *((T *) &(value->value));
            };

            template<typename Q =  T, typename std::enable_if<std::is_same<Q, std::string>::value>::type* = nullptr>
            std::string get() const {
                if (!isSet) {
                    std::cerr << "Option " << value->description << " is not set" << std::endl;
                }

                return value->value.stringValue;
            };
            template<typename Q =  T, typename std::enable_if<std::is_same<Q, std::vector<long>>::value>::type* = nullptr>
            std::vector<long> get() const {
                if (!isSet) {
                    std::cerr << "Option " << value->description << " is not set" << std::endl;
                }
                for(long i=1; i < value->value.longValues[0];++i) {
                    longValues->emplace_back(value->value.longValues[i]);
                }
                return *longValues;
            };
    };


        template<class T, typename std::enable_if<std::is_same<T, std::string>::value>::type* = nullptr>
        Option<std::string> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<std::string>(it->second, true);
            }
            return Option<std::string>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, double>::value>::type* = nullptr>
        Option<double> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<double>(it->second, true);
            }
            return Option<double>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, long>::value>::type* = nullptr>
        Option<long> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<long>(it->second, true);
            }
            return Option<long>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, std::vector<long>>::value>::type* = nullptr>
        Option<std::vector<long>> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<std::vector<long>>(it->second, true);
            }
            return Option<std::vector<long>>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr>
        Option<bool> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<bool>(it->second, true);
            }
            return Option<bool>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, LimonTypes::Vec4>::value>::type* = nullptr>
        Option<LimonTypes::Vec4> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<LimonTypes::Vec4>(it->second, true);
            }
            return Option<LimonTypes::Vec4>(nullptr, false);
        }

        template<class T, typename std::enable_if<std::is_same<T, LimonTypes::Mat4>::value>::type* = nullptr>
        Option<LimonTypes::Mat4> getOption(uint64_t optionHash) const {
            auto it = this->findOption(optionHash);
            if (it != this->options.end()) {
                return Option<LimonTypes::Mat4>(it->second, true);
            }
            return Option<LimonTypes::Mat4>(nullptr, false);
        }

        bool getOption(const std::string &optionName, uint32_t &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::LONG) {
                return false;
            }
            value = it->second->value.longValue;
            return true;
        }

        bool getOption(const std::string &optionName, long &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::LONG) {
                return false;
            }
            value = it->second->value.longValue;
            return true;
        }

        bool getOption(const std::string &optionName, bool &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::BOOLEAN) {
                return false;
            }
            value = it->second->value.longValue;
            return true;
        }

        bool getOption(const std::string &optionName, std::vector<long> &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::LONG_ARRAY) {
                return false;
            }
            value.clear();
            for (long i = 1; i < it->second->value.longValues[0]; ++i) {
                value.push_back(it->second->value.longValues[i]);
            }
            return true;
        }

        bool getOptionOrDefault(const std::string &optionName, long &value, long defaultValue) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::LONG) {
                value = defaultValue;
                return false;
            }
            value = it->second->value.longValue;
            return true;
        }

        bool getOptionOrDefault(const std::string &optionName, std::string &value, const std::string &defaultValue) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::STRING) {
                value = defaultValue;
                return false;
            }
            value = it->second->value.stringValue;
            return true;
        }

        bool getOption(const std::string &optionName, std::string &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::STRING) {
                return false;
            }
            value = it->second->value.stringValue;
            return true;
        }

        bool getOption(const std::string &optionName, double &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::DOUBLE) {
                return false;
            }
            value = it->second->value.doubleValue;
            return true;
        }

        bool getOption(const std::string &optionName, float &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::DOUBLE) {
                return false;
            }
            value = (float) it->second->value.doubleValue;
            return true;
        }

        bool getOptionOrDefault(const std::string &optionName, bool &value, bool defaultValue) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::BOOLEAN) {
                value = defaultValue;
                return false;
            }
            value = it->second->value.boolValue;
            return true;
        }

        bool getOption(const std::string &optionName, LimonTypes::Vec4 &value) const {
            auto it = this->options.find(optionName);
            if (it == this->options.end() || it->second->valueType != LimonTypes::GenericParameter::VEC4) {
                return false;
            }
            value = it->second->value.vectorValue;
            return true;
        }

        void setOption(const std::string &optionName, LimonTypes::Vec4 value) {
            auto it = this->options.find(optionName);
            if (it == this->options.end()) {
                std::shared_ptr<LimonTypes::GenericParameter> parameter = std::make_shared<LimonTypes::GenericParameter>();
                parameter->description = optionName;
                this->options[optionName] = parameter;
                it = this->options.find(optionName);
            }
            it->second->valueType = LimonTypes::GenericParameter::VEC4;
            it->second->value.vectorValue = value;
        }

        void setOption(const std::string &optionName, double value) {
            auto it = this->options.find(optionName);
            if (it == this->options.end()) {
                std::shared_ptr<LimonTypes::GenericParameter> parameter = std::make_shared<LimonTypes::GenericParameter>();
                parameter->description = optionName;
                this->options[optionName] = parameter;
                it = this->options.find(optionName);
            }
            it->second->valueType = LimonTypes::GenericParameter::DOUBLE;
            it->second->value.doubleValue = value;
        }

        void setOption(const std::string &optionName, long value) {
            auto it = this->options.find(optionName);
            if (it == this->options.end()) {
                std::shared_ptr<LimonTypes::GenericParameter> parameter = std::make_shared<LimonTypes::GenericParameter>();
                parameter->description = optionName;
                this->options[optionName] = parameter;
                it = this->options.find(optionName);
            }
            it->second->valueType = LimonTypes::GenericParameter::LONG;
            it->second->value.longValue = value;
        }

        void setOption(const std::string &optionName, bool value) {
            auto it = this->options.find(optionName);
            if (it == this->options.end()) {
                std::shared_ptr<LimonTypes::GenericParameter> parameter = std::make_shared<LimonTypes::GenericParameter>();
                parameter->description = optionName;
                this->options[optionName] = parameter;
                it = this->options.find(optionName);
            }
            it->second->valueType = LimonTypes::GenericParameter::BOOLEAN;
            it->second->value.boolValue = value;
        }

    private:
        Logger *logger{};
        std::unordered_map<std::string, std::shared_ptr<LimonTypes::GenericParameter>> options;

        /*SDL properties that should be available */
        void *imeWindowHandle;
        int drawableWidth, drawableHeight;
        int windowWidth, windowHeight;
        bool isWindowInFocus;

        bool loadVec3(tinyxml2::XMLNode *optionsNode, const std::string &name, glm::vec3 &);

        bool loadVec4(tinyxml2::XMLNode *optionsNode, const std::string &name, glm::vec4 &);

        bool loadDouble(tinyxml2::XMLNode *optionsNode, const std::string &name, double &);

        bool loadLong(tinyxml2::XMLNode *optionsNode, const std::string &name, long &);

        bool loadString(tinyxml2::XMLNode *optionsNode, const std::string &name, std::string &);

        bool loadBool(tinyxml2::XMLNode *optionsNode, const std::string &name, bool &);

    public:

        bool loadOptionsNew(const std::string &optionsFileName);

        void *getImeWindowHandle() const {
            return imeWindowHandle;
        }

        void setImeWindowHandle(void *imeWindowHandle) {
            Options::imeWindowHandle = imeWindowHandle;
        }

        int getDrawableWidth() const {
            return drawableWidth;
        }

        void setDrawableWidth(int drawableWidth) {
            Options::drawableWidth = drawableWidth;
        }

        int getDrawableHeight() const {
            return drawableHeight;
        }

        void setDrawableHeight(int drawableHeight) {
            Options::drawableHeight = drawableHeight;
        }

        int getWindowWidth() const {
            return windowWidth;
        }

        void setWindowWidth(int windowWidth) {
            Options::windowWidth = windowWidth;
        }

        int getWindowHeight() const {
            return windowHeight;
        }

        void setWindowHeight(int windowHeight) {
            Options::windowHeight = windowHeight;
        }

        bool isIsWindowInFocus() const {
            return isWindowInFocus;
        }

        void setIsWindowInFocus(bool isWindowInFocus) {
            Options::isWindowInFocus = isWindowInFocus;
        }

        uint32_t getScreenHeight() const {
            uint32_t height = 1080;//default
            getOption("screenHeight", height);
            return height;
        }

        void setScreenHeight(unsigned int height) {
            setOption("screenHeight", (long) height);
        }

        uint32_t getScreenWidth() const {
            uint32_t width = 1920;//default
            getOption("screenWidth", width);
            return width;
        }

        void setScreenWidth(unsigned int width) {
            setOption("screenWidth", (long) width);
        }

        Options() {
            this->logger = new Logger();
        };

        Logger *getLogger() {
            return logger;
        }

        const std::unordered_map<std::string, std::shared_ptr<LimonTypes::GenericParameter>> &getAllOptions() const {
            return this->options;
        }

    };

}
#endif //LIMONENGINE_OPTIONS_H
