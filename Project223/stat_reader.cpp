//чтение запросов на вывод и сам вывод
#include "stat_reader.h"

#include <iomanip>

namespace transport {

	StatRequestType UserStatData::getRequestType() const {
		return _request_type;
	}

	void UserStatData::setRequestType(StatRequestType rt) {
		_request_type = rt;
	}

	BusStatInputData::BusStatInputData(BusID bus_id) : _bus_id(std::move(bus_id)) {
		setRequestType(StatRequestType::BusStat);
	}

	BusID& BusStatInputData::getBusID() {
		return _bus_id;
	}

	StatReader StatReaderFactory::Create() {
		return StatReader();
	}

	std::vector<std::unique_ptr<UserStatData>> StatReader::getUserInput(std::istream& in) {
		std::vector<std::unique_ptr<UserStatData>> res;
		int ct = 0;
		in >> ct;
		res.reserve(ct);
		while (ct--) {
			std::string command;
			in >> command;
			if (command == "Bus") {
				BusID bid;
				in >> bid;
				res.push_back(std::make_unique<BusStatInputData>(std::move(bid)));
			}
			if (command == "Stop") {
				std::string stopName = getStopName(in);
				res.push_back(std::make_unique<StopStatInputData>(std::move(stopName)));
			}
		}
		return res;
	}

	void StatDataProcessor::Process(const TransportCatalogue& transport_catalog, std::vector<std::unique_ptr<UserStatData>> userStatData, std::ostream& out) {
		for (const std::unique_ptr<UserStatData>& data : userStatData) {
			StatRequestType rt = data->getRequestType();
			if (!_processes.count(rt)) {
				continue;
			}
			for (const auto& [key, value] : _processes.at(data->getRequestType())) {
				value(transport_catalog, data, out);
			}
		}
	}

	int StatDataProcessor::_ct = 0;

	int StatDataProcessor::RegisterProcess(StatRequestType rt, ProcessFn fn) {
		_processes[rt][_ct++] = fn;
		return _ct;
	}

	std::string StatReader::getStopName(std::istream& in) {
		std::string result;
		std::string tmp;
		std::getline(in, tmp);
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

	StopStatInputData::StopStatInputData(std::string stop_name) : _stop_name(stop_name) {
		setRequestType(StatRequestType::StopStat);
	}

	std::string& StopStatInputData::getStopName() {
		return _stop_name;
	}

	void ProcessBus(const TransportCatalogue& transport_catalog, const std::unique_ptr<UserStatData>& userStatData, std::ostream& out) {

		BusStatInputData* stopData = static_cast<BusStatInputData*>(userStatData.get());
		BusID& bid = stopData->getBusID();
		if (transport_catalog.isBusIDExists(bid)) {
			std::ios::fmtflags oldFlag = out.flags();

			out << std::setprecision(6);
			out << "Bus " << bid << ": ";

			const Route& rt = transport_catalog.findRouteByBusID(bid);
			out << (rt.isRouteCircle ? rt.stops.size() + 1 : rt.stops.size() + rt.stops.size() - 1) << " stops on route, ";
			out << rt.stops.size() << " unique stops, ";
			out << transport_catalog.routeLength(bid) << " route length";

			out.flags(oldFlag);
		}
		else {
			out << "Bus " << bid << ": not found";
		}
		out << std::endl;
	}

	void ProcessBusDistance(const TransportCatalogue& transport_catalog, const std::unique_ptr<UserStatData>& userStatData, std::ostream& out) {

		BusStatInputData* stopData = static_cast<BusStatInputData*>(userStatData.get());
		BusID& bid = stopData->getBusID();
		if (transport_catalog.isBusIDExists(bid)) {
			std::ios::fmtflags oldFlag = out.flags();

			out << std::setprecision(6);
			out << "Bus " << bid << ": ";

			const Route& rt = transport_catalog.findRouteByBusID(bid);
			out << (rt.isRouteCircle ? rt.stops.size() + 1 : rt.stops.size() + rt.stops.size() - 1) << " stops on route, ";
			out << rt.stops.size() << " unique stops, ";
			double d = transport_catalog.routeDistance(bid);
			double l = transport_catalog.routeLength(bid);
			out << d << " route length, ";
			out << (d / l) << " curvature";

			out.flags(oldFlag);
		}
		else {
			out << "Bus " << bid << ": not found";
		}
		out << std::endl;
	}

	void ProcessStop(const TransportCatalogue& transport_catalog, const std::unique_ptr<UserStatData>& userStatData, std::ostream& out) {

		StopStatInputData* stopData = static_cast<StopStatInputData*>(userStatData.get());
		std::string& stopName = stopData->getStopName();
		if (transport_catalog.isStopNameExists(stopName)) {
			std::vector<Trace> traces = transport_catalog.findTracesByStopName(stopName);
			if (traces.size() == 0) {
				out << "Stop " << stopName << ": not found";
			}
			else {
				std::ios::fmtflags oldFlag = out.flags();

				out << std::setprecision(6);
				out << "Stop " << stopName << ": buses";

				for (Trace trace : traces) {
					out << " "s << trace.bus_num;
				}

				out.flags(oldFlag);
			}
		}
		else {
			out << "Stop " << stopName << ": not found";
		}
		out << std::endl;
	}

	StatDataProcessor StatDataProcessorFactory::Create() {
		StatDataProcessor res;
		res.RegisterProcess(StatRequestType::BusStat, ProcessBusDistance);
		res.RegisterProcess(StatRequestType::StopStat, ProcessStop);
		return res;
	}
}