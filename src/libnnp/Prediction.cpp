// n2p2 - A neural network potential package
// Copyright (C) 2018 Andreas Singraber (University of Vienna)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Prediction.h"
#include <fstream>   // std::ifstream
#include <map>       // std::map
#include <stdexcept> // std::runtime_error
#include "utility.h"

using namespace std;
using namespace nnp;

Prediction::Prediction() : Mode(),
                           fileNameSettings        ("input.nn"          ),
                           fileNameScaling         ("scaling.data"      ),
                           formatWeightsFilesShort ("weights.%03zu.data" ),
                           formatWeightsFilesCharge("weightse.%03zu.data")
{
}

void Prediction::setup()
{
    initialize();
    loadSettingsFile(fileNameSettings);
    setupGeneric();
    setupSymmetryFunctionScaling(fileNameScaling);
    map<string, string> formatWeights {
        {"short", formatWeightsFilesShort},
        {"elec", formatWeightsFilesCharge}
    };
    setupNeuralNetworkWeights(formatWeights);
    if (nnpType == NNPType::HDNNP_4G) setupAtomicHardness();
    setupSymmetryFunctionStatistics(false, false, true, false);
}

void Prediction::readStructureFromFile(string const& fileName)
{
    ifstream file;
    file.open(fileName.c_str());
    structure.reset();
    structure.setElementMap(elementMap);
    structure.readFromFile(fileName);
    removeEnergyOffset(structure);
    if (normalize)
    {
        structure.toNormalizedUnits(meanEnergy, convEnergy, convLength);
    }
    file.close();

    return;
}

void Prediction::predict()
{
    structure.calculateNeighborList(maxCutoffRadius);
#ifdef NNP_NO_SF_GROUPS
    calculateSymmetryFunctions(structure, true);
#else
    calculateSymmetryFunctionGroups(structure, true);
#endif
    calculateAtomicNeuralNetworks(structure, true);
    if (nnpType == NNPType::HDNNP_4G)
    {
        chargeEquilibration(structure);
        calculateAtomicNeuralNetworks(structure, true, "short");
    }
    calculateEnergy(structure);
    if (nnpType == NNPType::HDNNP_4G ||
        nnpType == NNPType::HDNNP_Q) calculateCharge(structure);
    calculateForces(structure);
    if (normalize)
    {
        structure.toPhysicalUnits(meanEnergy, convEnergy, convLength);
    }
    addEnergyOffset(structure, false);
    addEnergyOffset(structure, true);

    return;
}