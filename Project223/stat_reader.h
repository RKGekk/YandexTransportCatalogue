//чтение запросов на вывод и сам вывод
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <utility>

#include "transport_catalogue.h"

namespace transport {

	using namespace std::literals;

	enum class StatRequestType {
		BusStat,
		StopStat
	};

	class UserStatData {
	public:
		StatRequestType getRequestType() const;
	protected:
		void setRequestType(StatRequestType rt);
	private:
		StatRequestType _request_type;
	};

	class BusStatInputData : public UserStatData {
	public:
		BusStatInputData(BusID BusID);
		BusID& getBusID();

	private:
		BusID _bus_id;
	};

	class StopStatInputData : public UserStatData {
	public:
		StopStatInputData(std::string stop_name);
		std::string& getStopName();

	private:
		std::string _stop_name;
	};

	class StatReader {
	public:
		std::vector<std::unique_ptr<UserStatData>> getUserInput(std::istream& in);
	private:
		std::string getStopName(std::istream& in);
	};

	class StatReaderFactory {
	public:
		static StatReader Create();
	};

	class StatDataProcessor {
	public:
		using ProcessFn = std::function<void(const TransportCatalogue&, const std::unique_ptr<UserStatData>&, std::ostream&)>;
		void Process(const TransportCatalogue& transport_catalog, std::vector<std::unique_ptr<UserStatData>>, std::ostream& out);
		int RegisterProcess(StatRequestType rt, ProcessFn fn);
	private:
		std::unordered_map<StatRequestType, std::unordered_map<int, ProcessFn>> _processes;
		static int _ct;
	};

	class StatDataProcessorFactory {
	public:
		static StatDataProcessor Create();
	};
}