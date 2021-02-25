//функции для работы с географическими координатами
#pragma once

#include <cmath>

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <limits>
#include <unordered_set>
#include <sstream>

namespace transport {

	template<typename C, typename T>
	auto insert_in_container(C& c, T&& t) -> decltype(c.push_back(std::forward<T>(t)), void()) {
		c.push_back(std::forward<T>(t));
	}

	template<typename C, typename T>
	auto insert_in_container(C& c, T&& t) -> decltype(c.insert(std::forward<T>(t)), void()) {
		c.insert(std::forward<T>(t));
	}

	template <typename Container>
	auto its_and_idx(Container&& container) {
		return std::tuple{ std::begin(container), std::end(container), 0 };
	}


	static inline std::string_view sub_string(std::string_view s, std::size_t p, std::size_t n = std::string_view::npos) {
		return s.substr(p, n);
	}

	static inline void ltrim(std::string& s) {
		s.erase(
			s.begin(),
			std::find_if(
				s.begin(),
				s.end(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
			)
		);
	}

	static inline void rtrim(std::string& s) {
		s.erase(
			std::find_if(
				s.rbegin(),
				s.rend(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
			).base(),
			s.end()
		);
	}

	static inline void mtrim(std::string& s) {
		std::string::iterator it = std::unique(s.begin(), s.end(), [](auto lhs, auto rhs) { return lhs == rhs && lhs == ' '; });
		s.erase(it, s.end());
	}

	static inline void trim(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

	static inline void ftrim(std::string& s) {
		ltrim(s);
		rtrim(s);
		mtrim(s);
	}

	template<typename Container>
	class Splitter {
	public:
		Splitter(std::string separatorVerb) : _separatorVerb(std::move(separatorVerb)) {};
		virtual Container Split(std::istream& in) = 0;
		virtual Container Split(std::string& in) = 0;
		virtual const std::string& getSeparatorVerb() { return _separatorVerb; }
	protected:
		std::string _separatorVerb;
	};

	template<typename Container>
	class RegExExtractor : public Splitter<Container> {
	private:
		Container splitR(const std::string& input, const std::string& regex) {
			std::regex re(regex);
			std::sregex_token_iterator first(input.begin(), input.end(), re, 0);
			std::sregex_token_iterator last;
			return Container(first, last);
		}

		Container splitR(std::istream& in, const std::string& regex) {
			std::string line;
			std::getline(in, line);
			return splitR(line, regex);
		}

	public:
		RegExExtractor(std::string separatorVerb) : Splitter<Container>(std::move(separatorVerb)) {};
		virtual Container Split(std::istream& in) override {
			return splitR(in, this->_separatorVerb);
		}
		virtual Container Split(std::string& in) override {
			return splitR(in, this->_separatorVerb);
		}
	};

	template<typename Container>
	class StlSplitter : public Splitter<Container> {
	private:
		Container splitR(const std::string& input, const std::string& delims) {
			Container out;
			size_t delims_len = delims.size();
			auto begIdx = 0;
			auto endIdx = input.find(delims, begIdx);
			if (endIdx == std::string::npos && input.size() != 0) {
				insert_in_container(out, input);
			}
			while (endIdx != std::string::npos) {
				insert_in_container(out, input.substr(begIdx, endIdx - begIdx));
				begIdx = endIdx + delims_len;
				endIdx = input.find(delims, begIdx);
				if (endIdx == std::string::npos) {
					insert_in_container(out, input.substr(begIdx, input.length() - begIdx));
				}
			}
			return out;
		}

		Container splitR(std::istream& in, const std::string& delims) {
			std::string line;
			std::getline(in, line);
			return splitR(line, delims);
		}

	public:
		StlSplitter(std::string separatorVerb) : Splitter<Container>(std::move(separatorVerb)) {};
		virtual Container Split(std::istream& in) override {
			return splitR(in, this->_separatorVerb);
		}
		virtual Container Split(std::string& in) override {
			return splitR(in, this->_separatorVerb);
		}
	};

	template<typename Container>
	class StlSplitterTrimmer : public Splitter<Container> {
	private:
		Container splitR(const std::string& input, const std::string& delims) {
			Container out;
			auto begIdx = input.find_first_not_of(delims);
			while (begIdx != std::string::npos) {
				auto endIdx = input.find_first_of(delims, begIdx);
				if (endIdx == std::string::npos) {
					endIdx = input.length();
				}
				std::string tmp = input.substr(begIdx, endIdx - begIdx);
				ftrim(tmp);
				insert_in_container(out, tmp);
				begIdx = input.find_first_not_of(delims, endIdx);
			}
			return out;
		}

		Container splitR(std::istream& in, const std::string& delims) {
			std::string line;
			std::getline(in, line);
			return splitR(line, delims);
		}

	public:
		StlSplitterTrimmer(std::string separatorVerb) : Splitter<Container>(std::move(separatorVerb)) {};
		virtual Container Split(std::istream& in) override {
			return splitR(in, this->_separatorVerb);
		}
		virtual Container Split(std::string& in) override {
			return splitR(in, this->_separatorVerb);
		}
	};

	struct Coordinates {
		double lat;
		double lng;
	};

	inline double ComputeDistance(Coordinates from, Coordinates to) {
		using namespace std;
		static constexpr double dr = 3.14159265358979323846 / 180.0;
		static constexpr double equatorial_radius = 6378.1370;
		static constexpr double distance_center_to_pole = 6356.7523;
		static constexpr double R = ((2.0 * equatorial_radius + distance_center_to_pole) / 3.0) * 1000.0; //WGS84
		double f1 = from.lat * dr;
		double sf1 = sin(f1);
		double cf1 = cos(f1);
		double f2 = to.lat * dr;
		double sf2 = sin(f2);
		double cf2 = cos(f2);
		double dl = abs(from.lng - to.lng) * dr;
		double sdl = sin(dl);
		double cdl = cos(dl);
		return (atan((sqrt((cf2 * sdl) * (cf2 * sdl) + (cf1 * sf2 - sf1 * cf2 * cdl) * (cf1 * sf2 - sf1 * cf2 * cdl))) / (sf1 * sf2 + cf1 * cf2 * cdl))) * R;
	}
}