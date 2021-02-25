//÷òåíèå çàïğîñîâ íà çàïîëíåíèå áàçû
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <limits>
#include <unordered_set>
#include <memory>
#include <functional>
#include <type_traits>

#include "geo.h"
#include "transport_catalogue.h"

namespace transport {

	using namespace std::literals;

	enum class InputRequestType {
		RouteStop,
		Bus
	};

	class UserInputData {
	public:
		InputRequestType getRequestType() const;
	protected:
		void setRequestType(InputRequestType rt);
	private:
		InputRequestType _request_type;
	};

	class RouteStopInputData : public UserInputData {
	public:
		RouteStopInputData(std::string stop_name, Coordinates coordinates, Distances distances);
		std::string& getStopName();
		Coordinates getCoordinates();
		Distances& getDistances();

	private:
		std::string _stop_name;
		Coordinates _coordinates;
		Distances _distances;
	};

	class BusInputData : public UserInputData {
	public:
		BusInputData(std::vector<std::string> stop_names, BusID bus_id, bool isCircle);
		std::vector<std::string>& getStopNames();
		BusID& getBusID();
		bool getIsCircle();
	private:
		std::vector<std::string> _stop_names;
		BusID _bus_id;
		bool _isCircle;
	};


	class InputReader {
	public:
		using name_container = std::vector<std::string>;
		using splitter_i = Splitter<name_container>;
		using strong_splitter = std::unique_ptr<splitter_i>;

		InputReader(strong_splitter stopNameSplitter, strong_splitter stopNameSplitterStraight, strong_splitter stopNameSplitterCircle, strong_splitter basicÑommaSplitter, strong_splitter distancesSplitter);

		std::vector<std::unique_ptr<UserInputData>> getUserInput(std::istream& in);

	private:
		std::string getStopName(std::istream& in);
		Coordinates getCoordinates(std::istream& in);
		Coordinates getCoordinates(const std::string& lat, const std::string& lng);

		strong_splitter _coordinatesSplitter;
		strong_splitter _distancesSplitter;
		strong_splitter _stopNameSplitterStraight;
		strong_splitter _stopNameSplitterCircle;
		strong_splitter _basicÑommaSplitter;
	};

	class InputReaderFactory {
	public:
		static InputReader Create();
	};

	class InputDataProcessor {
	public:
		static void Process(TransportCatalogue& transport_catalog, std::vector<std::unique_ptr<UserInputData>>);
	};
}