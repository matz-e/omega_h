#ifndef OMEGA_H_HPP
#define OMEGA_H_HPP

#include "omega_h.h"

/* standard C++ headers required to parse this file */

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

/* Kokkos include block */

#ifdef OSH_USE_KOKKOS
#ifdef __clang__
#pragma clang system_header
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#include <Kokkos_Core.hpp>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#endif //OSH_USE_KOKKOS

#ifdef OSH_USE_KOKKOS
#define OSH_INLINE KOKKOS_INLINE_FUNCTION
#else
#define OSH_INLINE inline
#endif //OSH_USE_KOKKOS

#ifdef OSH_USE_CUDA
#define OSH_DEVICE __device__ inline
#define OSH_LAMBDA [=]__device__
#else
#define OSH_DEVICE inline
#define OSH_LAMBDA [=]
#endif //OSH_USE_CUDA

namespace osh {

typedef std::int8_t  I8;
typedef std::int16_t I16;
typedef std::int32_t I32;
typedef std::int64_t I64;
typedef I32          Int;
typedef I32          LO;
typedef I64          GO;
typedef double       Real;

template <typename T>
class HostWrite;

template <typename T>
class Write {
#ifdef OSH_USE_KOKKOS
  Kokkos::View<T*> view_;
#else
  std::shared_ptr<T> ptr_;
  LO size_;
#endif
public:
  OSH_INLINE Write();
  Write(LO size);
  Write(LO size, T value);
  Write(LO size, T offset, T stride);
  Write(HostWrite<T> host_write);
  LO size() const;
  OSH_DEVICE T& operator[](LO i) const {
#ifdef OSH_CHECK_BOUNDS
    OSH_CHECK(0 <= i);
    OSH_CHECK(i < size());
#endif
#ifdef OSH_USE_KOKKOS
    return view_(i);
#else
    return ptr_.get()[i];
#endif
  }
  T* data() const;
#ifdef OSH_USE_KOKKOS
  Kokkos::View<T*> view() const;
#endif
  void set(LO i, T value) const;
  T get(LO i) const;
  bool exists() const;
};

template <typename T>
OSH_INLINE Write<T>::Write():
#ifdef OSH_USE_KOKKOS
  view_()
#else
  ptr_(),
  size_(0)
#endif
{
}

template <typename T>
class Read {
  Write<T> write_;
public:
  OSH_INLINE Read() {}
  Read(Write<T> write);
  Read(LO size, T value);
  Read(LO size, T offset, T stride);
  Read(std::initializer_list<T> l);
  LO size() const;
  OSH_DEVICE T const& operator[](LO i) const {
    return write_[i];
  }
  T const* data() const;
#ifdef OSH_USE_KOKKOS
  Kokkos::View<const T*> view() const;
#endif
  T get(LO i) const;
  T last() const;
  bool exists() const;
};

class LOs : public Read<LO> {
public:
  OSH_INLINE LOs() {}
  OSH_INLINE LOs(Read<LO> base):Read<LO>(base) {}
  LOs(Write<LO> write);
  LOs(LO size, LO value);
  LOs(LO size, LO offset, LO stride);
  LOs(std::initializer_list<LO> l);
};

template <typename T>
Read<T> permute(Read<T> a_data, LOs a2b, Int width);

class Reals : public Read<Real> {
public:
  Reals();
  OSH_INLINE Reals(Read<Real> base):Read<Real>(base) {}
  Reals(Write<Real> write);
  Reals(LO size, Real value);
  Reals(std::initializer_list<Real> l);
};

template <typename T>
class HostRead {
  Read<T> read_;
#ifdef OSH_USE_KOKKOS
  typename Kokkos::View<const T*>::HostMirror mirror_;
#endif
public:
  HostRead();
  HostRead(Read<T> read);
  LO size() const;
  inline T const& operator[](LO i) const {
#ifdef OSH_USE_KOKKOS
#ifdef OSH_CHECK_BOUNDS
    OSH_CHECK(0 <= i);
    OSH_CHECK(i < size());
#endif
    return mirror_(i);
#else
    return read_[i];
#endif
  }
  T const* data() const;
  T last() const;
};

template <typename T>
class HostWrite {
  Write<T> write_;
#ifdef OSH_USE_KOKKOS
  typename Kokkos::View<T*>::HostMirror mirror_;
#endif
public:
  HostWrite();
  HostWrite(LO size);
  HostWrite(LO size, T offset, T stride);
  HostWrite(Write<T> write);
  HostWrite(std::initializer_list<T> l);
  Write<T> write() const;
  LO size() const;
  inline T& operator[](LO i) const {
#ifdef OSH_USE_KOKKOS
#ifdef OSH_CHECK_BOUNDS
    OSH_CHECK(0 <= i);
    OSH_CHECK(i < size());
#endif
    return mirror_(i);
#else
    return write_[i];
#endif
  }
  T* data() const;
};

class TagBase {
  public:
    TagBase(std::string const& name, Int ncomps, osh_xfer xfer);
    virtual ~TagBase();
    std::string const& name() const;
    Int ncomps() const;
    osh_xfer xfer() const;
    virtual osh_type type() const = 0;
  private:
    std::string name_;
    Int ncomps_;
    osh_xfer xfer_;
};

template <typename T>
class Tag : public TagBase {
  public:
    Tag(std::string const& name, Int ncomps, osh_xfer xfer);
    Read<T> array() const;
    void set_array(Read<T> array);
    virtual osh_type type() const override;
  private:
    Read<T> array_;
};

struct Remotes {
  Remotes() {}
  Remotes(Read<I32> ranks_, LOs idxs_):
    ranks(ranks_),idxs(idxs_) {
  }
  Read<I32> ranks;
  LOs idxs;
};

struct Int128
{
  std::int64_t high;
  std::uint64_t low;
  OSH_INLINE Int128();
  OSH_INLINE Int128(std::int64_t h, std::uint64_t l);
  OSH_INLINE Int128(std::int64_t value);
  OSH_INLINE void operator=(Int128 const& rhs) volatile;
  OSH_INLINE Int128(Int128 const& rhs);
  OSH_INLINE Int128(const volatile Int128& rhs);
  double to_double(double unit) const;
  void print(std::ostream& o) const;
  static OSH_INLINE Int128 from_double(double value, double unit);
};

class Comm;

typedef std::shared_ptr<Comm> CommPtr;

class Comm {
#ifdef OSH_USE_MPI
  MPI_Comm impl_;
#endif
  Read<I32> srcs_;
  HostRead<I32> host_srcs_;
  Read<I32> dsts_;
  HostRead<I32> host_dsts_;
public:
  Comm();
#ifdef OSH_USE_MPI
  Comm(MPI_Comm impl);
#else
  Comm(bool sends_to_self);
#endif
  ~Comm();
  static CommPtr world();
  static CommPtr self();
  I32 rank() const;
  I32 size() const;
  CommPtr dup() const;
  CommPtr split(I32 color, I32 key) const;
  CommPtr graph(Read<I32> dsts) const;
  CommPtr graph_adjacent(Read<I32> srcs, Read<I32> dsts) const;
  CommPtr graph_inverse() const;
  Read<I32> sources() const;
  Read<I32> destinations() const;
  template <typename T>
  T allreduce(T x, osh_op op) const;
  bool reduce_or(bool x) const;
  bool reduce_and(bool x) const;
  Int128 add_int128(Int128 x) const;
  template <typename T>
  T exscan(T x, osh_op op) const;
  template <typename T>
  void bcast(T& x) const;
  void bcast_string(std::string& s) const;
  template <typename T>
  Read<T> allgather(T x) const;
  template <typename T>
  Read<T> alltoall(Read<T> x) const;
  template <typename T>
  Read<T> alltoallv(Read<T> sendbuf,
      Read<LO> sendcounts, Read<LO> sdispls,
      Read<LO> recvcounts, Read<LO> rdispls) const;
  void barrier() const;
};

class Dist {
  CommPtr parent_comm_;
  LOs roots2items_[2];
  LOs items2content_[2];
  LOs msgs2content_[2];
  CommPtr comm_[2];
public:
  Dist();
  Dist(CommPtr comm, Remotes fitems2rroots, LO nrroots);
  void set_parent_comm(CommPtr parent_comm);
  void set_dest_ranks(Read<I32> items2ranks);
  void set_dest_idxs(LOs fitems2rroots, LO nrroots);
  void set_roots2items(LOs froots2fitems);
  Dist invert() const;
  template <typename T>
  Read<T> exch(Read<T> data, Int width) const;
  template <typename T>
  Read<T> exch_reduce(Read<T> data, Int width, osh_op op) const;
  CommPtr parent_comm() const;
  CommPtr comm() const;
  LOs msgs2content() const;
  LOs content2msgs() const;
  LOs items2msgs() const;
  LOs roots2items() const;
  Read<I32> msgs2ranks() const;
  Read<I32> items2ranks() const;
  LOs items2dest_idxs() const;
  Remotes items2dests() const;
  LO nitems() const;
  LO nroots() const;
  LO ndests() const;
  LO nsrcs() const;
  void change_comm(CommPtr new_comm);
  Remotes exch(Remotes data, Int width) const;
private:
  enum { F, R };
};

struct Graph {
  Graph() {}
  explicit Graph(LOs ab2b_):ab2b(ab2b_) {}
  Graph(LOs a2ab_, LOs ab2b_):a2ab(a2ab_),ab2b(ab2b_) {}
  LOs a2ab;
  LOs ab2b;
};

enum { DIMS = 4 };

enum {
  VERT,
  EDGE,
  FACE,
  TRI = FACE,
  REGION,
  TET = REGION
};

struct Adj : public Graph {
  Adj() {}
  explicit Adj(LOs ab2b_):Graph(ab2b_) {}
  Adj(LOs ab2b_, Read<I8> codes_):Graph(ab2b_),codes(codes_) {}
  Adj(LOs a2ab_, LOs ab2b_, Read<I8> codes_):
    Graph(a2ab_, ab2b_),codes(codes_) {
  }
  Adj(LOs a2ab_, LOs ab2b_):
    Graph(a2ab_, ab2b_) {
  }
  Adj(Graph g):Graph(g) {}
  Read<I8> codes;
};

void find_matches(Int dim, LOs av2v, LOs bv2v, Adj v2b,
    LOs* a2b_out, Read<I8>* codes_out);

class Library {
  public:
    Library(Library const&) {}
    inline Library(int* argc, char*** argv) {
      osh_init(argc, argv);
    }
    ~Library();
    CommPtr world() const;
    CommPtr self() const;
};

namespace inertia {
struct Rib;
}

enum Partition {
  ELEMENT_BASED,
  GHOSTED,
  VERTEX_BASED
};

class Mesh {
  public:
    Mesh();
    void set_comm(CommPtr comm);
    void set_dim(Int dim);
    void set_verts(LO nverts);
    void set_ents(Int dim, Adj down);
    void keep_canonical_globals(bool yn);
    CommPtr comm() const;
    Partition partition() const;
    Int dim() const;
    LO nents(Int dim) const;
    LO nelems() const;
    LO nverts() const;
    LO nedges() const;
    GO nglobal_ents(Int dim);
    template <typename T>
    void add_tag(Int dim, std::string const& name, Int ncomps,
        osh_xfer xfer);
    template <typename T>
    void add_tag(Int dim, std::string const& name, Int ncomps,
        osh_xfer xfer, Read<T> array);
    template <typename T>
    void set_tag(Int dim, std::string const& name, Read<T> array);
    template <typename T>
    Tag<T> const* get_tag(Int dim, std::string const& name) const;
    template <typename T>
    Read<T> get_array(Int dim, std::string const& name) const;
    void remove_tag(Int dim, std::string const& name);
    bool has_tag(Int dim, std::string const& name) const;
    Int ntags(Int dim) const;
    TagBase const* get_tag(Int dim, Int i) const;
    bool has_ents(Int dim) const;
    bool has_adj(Int from, Int to) const;
    Adj get_adj(Int from, Int to) const;
    Adj ask_down(Int from, Int to);
    LOs ask_verts_of(Int dim);
    Adj ask_up(Int from, Int to);
    Graph ask_star(Int dim);
    Graph ask_dual();
  public:
    typedef std::shared_ptr<TagBase> TagPtr;
    typedef std::shared_ptr<Adj> AdjPtr;
    typedef std::shared_ptr<Dist> DistPtr;
    typedef std::shared_ptr<inertia::Rib> RibPtr;
  private:
    typedef std::vector<TagPtr> TagVector;
    typedef TagVector::iterator TagIter;
    typedef TagVector::const_iterator TagCIter;
    TagIter tag_iter(Int dim, std::string const& name);
    TagCIter tag_iter(Int dim, std::string const& name) const;
    void check_dim(Int dim) const;
    void check_dim2(Int dim) const;
    void add_adj(Int from, Int to, Adj adj);
    Adj derive_adj(Int from, Int to);
    Adj ask_adj(Int from, Int to);
    void react_to_set_tag(Int dim, std::string const& name);
    Int dim_;
    CommPtr comm_;
    Int partition_;
    LO nents_[DIMS];
    TagVector tags_[DIMS];
    AdjPtr adjs_[DIMS][DIMS];
    Remotes owners_[DIMS];
    DistPtr dists_[DIMS];
    RibPtr rib_hints_;
    bool keeps_canonical_globals_;
  public:
    void add_coords(Reals array);
    Reals coords() const;
    void set_coords(Reals array);
    Read<GO> ask_globals(Int dim);
    void reset_globals();
    Reals ask_edge_lengths();
    Reals ask_qualities();
    void set_owners(Int dim, Remotes owners);
    Remotes ask_owners(Int dim);
    Read<I8> owned(Int dim);
    Dist ask_dist(Int dim);
    void set_partition(Partition partition, bool verbose = false);
    void migrate(Remotes new_elems2old_owners, bool verbose = false);
    void reorder();
    void balance();
    Graph ask_graph(Int from, Int to);
    template <typename T>
    Read<T> sync_array(Int ent_dim, Read<T> a, Int width);
    template <typename T>
    Read<T> sync_subset_array(Int ent_dim,
        Read<T> a_data, LOs a2e, T default_val, Int width);
    template <typename T>
    Read<T> reduce_array(Int ent_dim, Read<T> a, Int width, osh_op op);
    bool operator==(Mesh& other);
    Real min_quality();
    bool could_be_shared(Int ent_dim) const;
    bool owners_have_all_upward(Int ent_dim) const;
    Mesh copy_meta() const;
    bool keeps_canonical_globals() const;
    RibPtr rib_hints() const;
    void set_rib_hints(RibPtr hints);
};

namespace gmsh {
void read(std::istream& stream, Library const& lib, Mesh* mesh);
void read(std::string const& filename, Library const& lib, Mesh* mesh);
}

namespace vtk {
void write_vtu(std::ostream& stream, Mesh* mesh, Int cell_dim);
void write_vtu(std::string const& filename, Mesh* mesh, Int cell_dim);
void write_parallel(std::string const& path, Mesh* mesh, Int cell_dim);
class Writer {
    Mesh* mesh_;
    std::string root_path_;
    Int cell_dim_;
    std::vector<Real> times_;
  public:
    Writer(Mesh* mesh, std::string const& root_path, Int cell_dim);
    Writer(Writer const& other);
    ~Writer();
    void write(Real time);
    void write();
};
class FullWriter {
    std::vector<Writer> writers_;
  public:
    FullWriter(Mesh* mesh, std::string const& root_path);
    ~FullWriter();
    void write(Real time);
    void write();
};
} // end namespace vtk

/* returns true if the mesh was modified. */
bool adapt(Mesh* mesh,
    Real qual_floor,
    Real qual_ceil,
    Real len_floor,
    Real len_ceil,
    Int nlayers,
    Int verbosity);

namespace binary {
void write(std::string const& path, Mesh* mesh);
void read(std::string const& path, CommPtr comm, Mesh* mesh);
}

enum MeshComparison {
  SAME_MESH,
  SUPERSET_MESH,
  DIFFERENT_MESH
};

MeshComparison
compare_meshes(Mesh* a, Mesh* b, Real tol, Real floor, bool verbose);
bool check_regression(std::string const& prefix, Mesh* mesh,
    Real tol, Real floor);

void build_from_elems2verts(Mesh* mesh,
    CommPtr comm, Int edim, LOs ev2v, Read<GO> vert_globals);
void build_from_elems2verts(Mesh* mesh,
    Library const& lib, Int edim, LOs ev2v, LO nverts);
void build_from_elems_and_coords(Mesh* mesh,
    Library const& lib, Int edim, LOs ev2v, Reals coords);
void build_box(Mesh* mesh,
    Library const& lib,
    Real x, Real y, Real z,
    LO nx, LO ny, LO nz);

Real repro_sum(Reals a);
Real repro_sum(CommPtr comm, Reals a);
void repro_sum(CommPtr comm, Reals a, Int ncomps, Real result[]);

OSH_INLINE bool code_is_flipped(I8 code) {
  return code & 1;
}

OSH_INLINE Int code_rotation(I8 code) {
  return (code >> 1) & 3;
}

OSH_INLINE Int code_which_down(I8 code) {
  return (code >> 3);
}

} //end namespace osh

#endif
