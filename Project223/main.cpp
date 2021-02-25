#include <iostream>
#include <sstream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace transport;

int main() {

	{
		TransportCatalogue tc;
		tc.addRouteStop("Marushkino", Coordinates{ 55.595884, 37.209755 }, {});
		tc.addRoute("256"s, Route{ {"Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya"}, true });
		tc.addRoute("750"s, Route{ {"Tolstopaltsevo", "Marushkino", "Rasskazovka"}, false });
		tc.addRouteStop("Rasskazovka", Coordinates{ 55.632761, 37.333324 }, {});
		tc.addRouteStop("Biryulyovo Zapadnoye", Coordinates{ 55.574371, 37.651700 }, {});
		tc.addRouteStop("Biryusinka", Coordinates{ 55.581065, 37.648390 }, {});
		tc.addRouteStop("Universam", Coordinates{ 55.587655, 37.645687 }, {});
		tc.addRouteStop("Biryulyovo Tovarnaya", Coordinates{ 55.592028, 37.653656 }, {});
		tc.addRouteStop("Biryulyovo Passazhirskaya", Coordinates{ 55.580999, 37.659164 }, {});
	}

	//{
	//	TransportCatalogue tc;
	//	std::stringstream myString;
	//	myString << "10\nStop Tolstopaltsevo: 55.611087, 37.208290\nStop Marushkino: 55.595884, 37.209755\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\nBus 750: Tolstopaltsevo - Marushkino - Rasskazovka\nStop Rasskazovka: 55.632761, 37.333324\nStop Biryulyovo Zapadnoye: 55.574371, 37.651700\nStop Biryusinka: 55.581065, 37.648390\nStop Universam: 55.587655, 37.645687\nStop Biryulyovo Tovarnaya: 55.592028, 37.653656\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164";
	//	InputData(tc, myString);
	//}

	/*{
		TransportCatalogue tc;
		std::stringstream myString;
		myString << "10\nStop Tolstopaltsevo: 55.611087, 37.208290\nStop Marushkino: 55.595884, 37.209755\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\nBus 750: Tolstopaltsevo - Marushkino - Rasskazovka\nStop Rasskazovka: 55.632761, 37.333324\nStop Biryulyovo Zapadnoye: 55.574371, 37.651700\nStop Biryusinka: 55.581065, 37.648390\nStop Universam: 55.587655, 37.645687\nStop Biryulyovo Tovarnaya: 55.592028, 37.653656\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n3\nBus 256\nBus 750\nBus 751";
		InputData(tc, myString);
		OutputData(tc, myString, std::cout);
	}*/
	{
		TransportCatalogue tc;
		std::stringstream myString;
		//myString << "10\nStop Tolstopaltsevo: 55.611087, 37.208290\nStop Marushkino: 55.595884, 37.209755\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\nBus 750: Tolstopaltsevo - Marushkino - Rasskazovka\nStop Rasskazovka: 55.632761, 37.333324\nStop Biryulyovo Zapadnoye: 55.574371, 37.651700\nStop Biryusinka: 55.581065, 37.648390\nStop Universam: 55.587655, 37.645687\nStop Biryulyovo Tovarnaya: 55.592028, 37.653656\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n3\nBus 256\nBus 750\nBus 751";
		//myString << "13\nStop Tolstopaltsevo: 55.611087, 37.20829\nStop Marushkino: 55.595884, 37.209755\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\nBus 750: Tolstopaltsevo - Marushkino - Rasskazovka\nStop Rasskazovka: 55.632761, 37.333324\nStop Biryulyovo Zapadnoye: 55.574371, 37.6517\nStop Biryusinka: 55.581065, 37.64839\nStop Universam: 55.587655, 37.645687\nStop Biryulyovo Tovarnaya: 55.592028, 37.653656\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164\nBus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\nStop Rossoshanskaya ulitsa: 55.595579, 37.605757\nStop Prazhskaya: 55.611678, 37.603831\n6\nBus 256\nBus 750\nBus 751\nStop Samara\nStop Prazhskaya\nStop Biryulyovo Zapadnoye\n";
		myString << "13\nStop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\nStop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\nBus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka\nStop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino\nStop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\nStop Biryusinka: 55.581065, 37.64839, 750m to Universam\nStop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\nStop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\nBus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\nStop Rossoshanskaya ulitsa: 55.595579, 37.605757\nStop Prazhskaya: 55.611678, 37.603831\n6\nBus 256\nBus 750\nBus 751\nStop Samara\nStop Prazhskaya\nStop Biryulyovo Zapadnoye\n";

		InputReader inputReader = InputReaderFactory::Create();
		std::vector<std::unique_ptr<UserInputData>> inputData = inputReader.getUserInput(myString);
		InputDataProcessor::Process(tc, std::move(inputData));

		StatReader statReader = StatReaderFactory::Create();
		std::vector<std::unique_ptr<UserStatData>> statData = statReader.getUserInput(myString);

		StatDataProcessor proc = StatDataProcessorFactory::Create();
		proc.Process(tc, std::move(statData), std::cout);
	}

	return 0;
}