#ifndef OMEGA_H_HYPERCUBE_HPP
#define OMEGA_H_HYPERCUBE_HPP

#include <Omega_h_template_up.hpp>
#include <Omega_h_kokkos.hpp>

//DEBUG REMOVE NOW
#include <iostream>

/*! \file Omega_h_hypercube.hpp
  \brief Describes the canonical local boundary connectivity
         orderings for a single hypercube, and other related properties
  \details A hypercube is a generalization of a square in arbitrary dimensions.
           In Omega_h the relevant hypercubes are vertices, edges, quadrilaterals, and hexahedra.
  */

namespace Omega_h {

/* TODO: make these constexpr, either with C++14 or lots of
   ternary operators */

/*! \brief Relates bounding hypercube vertices the parent hypercube's vertices
  \param elem_dim The parent hypercube's dimension
  \param bdry_dim The bounding hypercube's dimension
  \param which_bdry The parent-local index of the bounding hypercube
  \param which_vert The bounding-local index of the vertex
  \returns The parent-local index of the vertex
  */
OMEGA_H_INLINE Int hypercube_down_template(
    Int elem_dim, Int bdry_dim, Int which_bdry, Int which_vert) {
  switch (elem_dim) {
    case 1:
      switch (bdry_dim) {
        case 0:
          return which_bdry;
      }
    case 2:
      switch (bdry_dim) {
        case 0:
          return which_bdry;
        case 1:
          switch (which_bdry) {
            case 0:
              switch (which_vert) {
                case 0:
                  return 0;
                case 1:
                  return 1;
              }
            case 1:
              switch (which_vert) {
                case 0:
                  return 1;
                case 1:
                  return 2;
              }
            case 2:
              switch (which_vert) {
                case 0:
                  return 2;
                case 1:
                  return 3;
              }
            case 3:
              switch (which_vert) {
                case 0:
                  return 3;
                case 1:
                  return 0;
              }
          }
      }
    case 3:
      switch (bdry_dim) {
        case 0:
          return which_bdry;
        case 1:
          switch (which_bdry) {
            case 0:
              switch (which_vert) {
                case 0:
                  return 0;
                case 1:
                  return 1;
              }
            case 1:
              switch (which_vert) {
                case 0:
                  return 1;
                case 1:
                  return 2;
              }
            case 2:
              switch (which_vert) {
                case 0:
                  return 2;
                case 1:
                  return 3;
              }
            case 3:
              switch (which_vert) {
                case 0:
                  return 3;
                case 1:
                  return 0;
              }
            case 4:
              switch (which_vert) {
                case 0:
                  return 0;
                case 1:
                  return 4;
              }
            case 5:
              switch (which_vert) {
                case 0:
                  return 1;
                case 1:
                  return 5;
              }
            case 6:
              switch (which_vert) {
                case 0:
                  return 2;
                case 1:
                  return 6;
              }
            case 7:
              switch (which_vert) {
                case 0:
                  return 3;
                case 1:
                  return 7;
              }
            case 8:
              switch (which_vert) {
                case 0:
                  return 4;
                case 1:
                  return 5;
              }
            case 9:
              switch (which_vert) {
                case 0:
                  return 5;
                case 1:
                  return 6;
              }
            case 10:
              switch (which_vert) {
                case 0:
                  return 6;
                case 1:
                  return 7;
              }
            case 11:
              switch (which_vert) {
                case 0:
                  return 7;
                case 1:
                  return 4;
              }
          }
        case 2:
          switch (which_bdry) {
            case 0:
              switch (which_vert) {
                case 0:
                  return 0;
                case 1:
                  return 3;
                case 2:
                  return 2;
                case 3:
                  return 1;
              }
            case 1:
              switch (which_vert) {
                case 0:
                  return 0;
                case 1:
                  return 1;
                case 2:
                  return 5;
                case 3:
                  return 4;
              }
            case 2:
              switch (which_vert) {
                case 0:
                  return 1;
                case 1:
                  return 2;
                case 2:
                  return 6;
                case 3:
                  return 5;
              }
            case 3:
              switch (which_vert) {
                case 0:
                  return 2;
                case 1:
                  return 3;
                case 2:
                  return 7;
                case 3:
                  return 6;
              }
            case 4:
              switch (which_vert) {
                case 0:
                  return 3;
                case 1:
                  return 0;
                case 2:
                  return 4;
                case 3:
                  return 7;
              }
            case 5:
              switch (which_vert) {
                case 0:
                  return 4;
                case 1:
                  return 5;
                case 2:
                  return 6;
                case 3:
                  return 7;
              }
          }
      }
  }
  return -1;
}

OMEGA_H_INLINE TemplateUp hypercube_up_template(
    Int elem_dim, Int bdry_dim, Int which_bdry, Int which_up) {
  switch (elem_dim) {
    case 3:
      switch (bdry_dim) {
        case 0:
          switch (which_bdry) {
            case 0:
              switch (which_up) {
                case 0:
                  return {0, 0, 0};
                case 1:
                  return {3, 1, 0};
                case 2:
                  return {4, 0, 0};
              }
            case 1:
              switch (which_up) {
                case 0:
                  return {1, 0, 0};
                case 1:
                  return {0, 1, 0};
                case 2:
                  return {5, 0, 0};
              }
            case 2:
              switch (which_up) {
                case 0:
                  return {2, 0, 0};
                case 1:
                  return {1, 1, 0};
                case 2:
                  return {6, 0, 0};
              }
            case 3:
              switch (which_up) {
                case 0:
                  return {3, 0, 0};
                case 1:
                  return {2, 1, 0};
                case 2:
                  return {7, 0, 0};
              }
            case 4:
              switch (which_up) {
                case 0:
                  return {8, 0, 0};
                case 1:
                  return {11, 1, 0};
                case 2:
                  return {4, 1, 0};
              }
            case 5:
              switch (which_up) {
                case 0:
                  return {9, 0, 0};
                case 1:
                  return {8, 1, 0};
                case 2:
                  return {5, 1, 0};
              }
            case 6:
              switch (which_up) {
                case 0:
                  return {10, 0, 0};
                case 1:
                  return {9, 1, 0};
                case 2:
                  return {6, 1, 0};
              }
            case 7:
              switch (which_up) {
                case 0:
                  return {11, 0, 0};
                case 1:
                  return {10, 1, 0};
                case 2:
                  return {7, 1, 0};
              }
          }
        case 1:
          switch (which_bdry) {
            case 0:
              switch (which_up) {
                case 0:
                  return {0, 3, 1};
                case 1:
                  return {1, 0, 0};
              }
            case 1:
              switch (which_up) {
                case 0:
                  return {0, 2, 1};
                case 1:
                  return {2, 0, 0};
              }
            case 2:
              switch (which_up) {
                case 0:
                  return {0, 1, 1};
                case 1:
                  return {3, 0, 0};
              }
            case 3:
              switch (which_up) {
                case 0:
                  return {0, 0, 1};
                case 1:
                  return {4, 0, 0};
              }
            case 4:
              switch (which_up) {
                case 0:
                  return {1, 3, 1};
                case 1:
                  return {4, 1, 0};
              }
            case 5:
              switch (which_up) {
                case 0:
                  return {2, 3, 1};
                case 1:
                  return {1, 1, 0};
              }
            case 6:
              switch (which_up) {
                case 0:
                  return {3, 3, 1};
                case 1:
                  return {2, 1, 0};
              }
            case 7:
              switch (which_up) {
                case 0:
                  return {4, 3, 1};
                case 1:
                  return {3, 1, 0};
              }
            case 8:
              switch (which_up) {
                case 0:
                  return {5, 0, 0};
                case 1:
                  return {1, 2, 1};
              }
            case 9:
              switch (which_up) {
                case 0:
                  return {5, 1, 0};
                case 1:
                  return {2, 2, 1};
              }
            case 10:
              switch (which_up) {
                case 0:
                  return {5, 2, 0};
                case 1:
                  return {3, 2, 1};
              }
            case 11:
              switch (which_up) {
                case 0:
                  return {5, 3, 0};
                case 1:
                  return {4, 2, 1};
              }
          }
      }
    case 2:
      switch (bdry_dim) {
        case 0:
          switch (which_bdry) {
            case 0:
              switch (which_up) {
                case 0:
                  return {3, 1, 0};
                case 1:
                  return {0, 0, 0};
              }
            case 1:
              switch (which_up) {
                case 0:
                  return {0, 1, 0};
                case 1:
                  return {1, 0, 0};
              }
            case 2:
              switch (which_up) {
                case 0:
                  return {1, 1, 0};
                case 1:
                  return {2, 0, 0};
              }
            case 3:
              switch (which_up) {
                case 0:
                  return {2, 1, 0};
                case 1:
                  return {3, 0, 0};
              }
          }
      }
  }
  return {-1, -1, true};
};

OMEGA_H_INLINE Int hypercube_degree(Int from_dim, Int to_dim) {
  switch (from_dim) {
    case 0:
      return 1;
    case 1:
      switch (to_dim) {
        case 0:
          return 2;
        case 1:
          return 1;
      }
    case 2:
      switch (to_dim) {
        case 0:
          return 4;
        case 1:
          return 4;
        case 2:
          return 1;
      }
    case 3:
      switch (to_dim) {
        case 0:
          return 8;
        case 1:
          return 12;
        case 2:
          return 6;
        case 3:
          return 1;
      }
  }
  return -1;
}

}  // end namespace Omega_h

#endif

