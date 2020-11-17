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

#ifndef SYMFNCCOMPANGNWEIGHTED_H
#define SYMFNCCOMPANGNWEIGHTED_H

#include "SymFncBaseCompAngWeighted.h"
#include <cstddef> // std::size_t
#include <string>  // std::string
#include <vector>  // std::vector

namespace nnp
{

struct Atom;
class ElementMap;

/** Angular symmetry function with polynomials (type 24)
 *
 * @f[
   G^{24}_i = \sum_{\substack{j,k\neq i \\ j < k}}
              Z_j Z_k
              C_{\text{rad}}(r_{ij})
              C_{\text{rad}}(r_{ik})
              C_{\text{rad}}(r_{jk})
              C_{\text{ang}}(\theta_{ijk})
 * @f]
 * Parameter string:
 * ```
 * <element-central> 24 <rlow> <rcutoff> <left> <right> <subtype>
 * ```
 * where
 * - `<element-central> .....` element symbol of central atom
 * - `<rlow>.................` lower radial boundary
 * - `<rcutoff> .............` upper radial boundary
 * - `<left> ................` left angle boundary 
 * - `<right> ...............` right angle boundary 
 * - `<subtype> .............` compact function specifier 
 *
 * See the description of SymFncBaseComp::setCompactFunction() for possible
 * values of the `<subtype>` argument.
 */
class SymFncCompAngnWeighted : public SymFncBaseCompAngWeighted
{
public:
    /** Constructor, sets type = 24
    */
    SymFncCompAngnWeighted(ElementMap const& elementMap);
    /** Overload == operator.
     */
    virtual bool operator==(SymFnc const& rhs) const;
    /** Overload < operator.
     */
    virtual bool operator<(SymFnc const& rhs) const;
    /** Calculate symmetry function for one atom.
     *
     * @param[in,out] atom Atom for which the symmetry function is caluclated.
     * @param[in] derivatives If also symmetry function derivatives will be
     *                        calculated and saved.
     */
    virtual void calculate(Atom& atom, bool const derivatives) const;
};

}

#endif
