#ifndef OMEGA_H_TRANSFER_FACE_HPP
#define OMEGA_H_TRANSFER_FACE_HPP

#include <Omega_h_adapt.hpp>
#include <Omega_h_adj.hpp>
#include <Omega_h_tag.hpp>

namespace Omega_h {
  void transer_div_free_face_flux(      Mesh *source_mesh, 
				        Mesh *target_mesh,
				  const Int key_dim,
				  const LOs source_keys,
				  const LOs keys2prods,
				  const LOs prods2target_elements,
				  const Read<Real> sourceFluxes,
				        Write<Real> targetFluxes );
}  // end namespace Omega_h

#endif