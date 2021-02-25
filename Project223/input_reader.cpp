//÷òåíèå çàïðîñîâ íà çàïîëíåíèå áàçû
#include "input_reader.h"

namespace transport {

	InputRequestType UserInputData::getRequestType() const {
		return _request_type;
	}

	void UserInputData::setRequestType(InputRequestType rt) {
		_request_type = rt;
	}

	InputReader InputReaderFactory::Create() {
		return InputReader(
			std::make_unique<RegExExtractor<InputReader::name_container>>("[0-9]+([.][0-9]*)*"s),
			std::make_unique<StlSplitterTrimmer<InputReader::name_container>>("-"s),
			std::make_unique<StlSplitterTrimmer<InputReader::name_container>>(">"s),
			std::make_unique<StlSplitter<InputReader::name_container>>(","s),
			std::make_unique<StlSplitter<InputReader::name_container>>(" to "s)
		);
	}

	InputReader::InputReader(strong_splitter coordinatesSplitter, strong_splitter stopNameSplitterStraight, strong_splitter stopNameSplitterCircle, strong_splitter basicÑommaSplitter, strong_splitter distancesSplitter) : _coordinatesSplitter(std::move(coordinatesSplitter)), _stopNameSplitterStraight(std::move(stopNameSplitterStraight)), _stopNameSplitterCircle(std::move(stopNameSplitterCircle)), _basicÑommaSplitter(std::move(basicÑommaSplitter)), _distancesSplitter(std::move(distancesSplitter)) {}

	std::vector<std::unique_ptr<UserInputData>> InputReader::getUserInput(std::istream& in) {

		std::vector<std::unique_ptr<UserInputData>> res;

		int ct = 0;
		in >> ct;
		res.reserve(ct);
		while (ct--) {
			std::string command;
			in >> command;
			if (command == "Stop") {
				std::string stopName = getStopName(in);
				std::vector<std::string> s = _basicÑommaSplitter->Split(in);
				Distances distances;
				if (s.size() == 2) {
					res.push_back(std::make_unique<RouteStopInputData>(std::move(stopName), getCoordinates(s[0], s[1]), std::move(distances)));
				}
				else {
					Coordinates c = getCoordinates(s[0], s[1]);
					for (auto it = s.begin() + 2; it != s.end(); ++it) {
						std::vector<std::string> ds = _distancesSplitter->Split(*it);
						std::string stopName = ds[1];
						ftrim(stopName);
						if constexpr (std::is_integral_v<dist>) { distances.insert({ stopName, std::stoi(ds[0]) }); }
						if constexpr (std::is_floating_point_v<dist>) { distances.insert({ stopName, std::stod(ds[0]) }); }
					}
					res.push_back(std::make_unique<RouteStopInputData>(std::move(stopName), c, std::move(distances)));
				}
			}
			else if (command == "Bus") {
				BusID bid = getStopName(in);
				std::string line;
				std::getline(in, line);
				if (line.find_first_of(_stopNameSplitterStraight->getSeparatorVerb()) != std::string::npos) {
					res.push_back(std::make_unique<BusInputData>(_stopNameSplitterStraight->Split(line), bid, false));
				}
				else {
					res.push_back(std::make_unique<BusInputData>(_stopNameSplitterCircle->Split(line), bid, true));
				}
			}
		}

		return res;
	}

	std::string InputReader::getStopName(std::istream& in) {
		std::string result;
		std::string tmp;
		std::getline(in, tmp, ':');
		std::stringstream sstream;
		sstream << tmp;
		std::string stopPart;
		sstream >> stopPart;
		result += stopPart;
		while (sstream >> stopPart) {
			result += " "s + stopPart;
		}
		return result;
	}

	Coordinates InputReader::getCoordinates(std::istream& in) {
		std::vector<std::string> nums = _coordinatesSplitter->Split(in);
		return Coordinates{ std::stod(nums[0]), std::stod(nums[1]) };
	}

	Coordinates InputReader::getCoordinates(const std::string& lat, const std::string& lng) {
		return Coordinates{ std::stod(lat), std::stod(lng) };
	}

	void InputDataProcessor::Process(TransportCatalogue& transport_catalog, std::vector<std::unique_ptr<UserInputData>> userInputData) {
		for (std::unique_ptr<UserInputData>& data : userInputData) {
			if (data->getRequestType() == InputRequestType::RouteStop) {
				RouteStopInputData* stopData = static_cast<RouteStopInputData*>(data.get());
				transport_catalog.addRouteStop(stopData->getStopName(), stopData->getCoordinates(), std::move(stopData->getDistances()));
			}
			if (data->getRequestType() == InputRequestType::Bus) {
				BusInputData* busData = static_cast<BusInputData*>(data.get());
				transport_catalog.addRoute(std::move(busData->getBusID()), std::move(busData->getStopNames()), busData->getIsCircle());
			}
		}
	}

	RouteStopInputData::RouteStopInputData(std::string stop_name, Coordinates coordinates, Distances distances) : _stop_name(std::move(stop_name)), _coordinates(coordinates), _distances(std::move(distances)) {
		setRequestType(InputRequestType::RouteStop);
	}

	std::string& RouteStopInputData::getStopName() {
		return _stop_name;
	}

	Coordinates RouteStopInputData::getCoordinates() {
		return _coordinates;
	}

	Distances& RouteStopInputData::getDistances() {
		return _distances;
	}

	BusInputData::BusInputData(std::vector<std::string> stop_names, BusID bus_id, bool isCircle) : _stop_names(std::move(stop_names)), _bus_id(std::move(bus_id)), _isCircle(isCircle) {
		setRequestType(InputRequestType::Bus);
	}

	std::vector<std::string>& BusInputData::getStopNames() {
		return _stop_names;
	}

	BusID& BusInputData::getBusID() {
		return _bus_id;
	}

	bool BusInputData::getIsCircle() {
		return _isCircle;
	}
}