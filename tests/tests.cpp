/// Catch2 test suite for vectorfontstorm
///
/// Tests are organised into sections that mirror the public API:
///   * aligntype         – enumeration values
///   * buffer_data       – simple vertex/index container
///   * glyph             – direct glyph construction and whitespace handling
///   * font (integration)– geometry pipeline exercised through font<T>
///
/// The string<T> and buffer<T> classes are not tested here because they
/// require a live OpenGL context; their GL calls (glGenBuffers etc.) would
/// segfault without one.  The geometry-generation path they rely on is
/// covered by the font integration tests.

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
// Pull in library headers – deliberately exclude string.h and buffer.h so
// that no OpenGL call-sites are instantiated.
#include "vectorfontstorm/aligntype.h"
#include "vectorfontstorm/buffer_data.h"
#include "vectorfontstorm/font.h"
#include "vectorfontstorm/glyph.h"

// ---------------------------------------------------------------------------
// Minimal vertex type that satisfies the (float, float, float) constructor
// requirement used throughout the library's geometry-generation templates.
// ---------------------------------------------------------------------------
struct test_vertex {
  float x, y, z;
  test_vertex(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

// ---------------------------------------------------------------------------
// Helper: load a font file into a byte buffer.
// Returns an empty vector if the file cannot be opened or read.
// ---------------------------------------------------------------------------
static std::vector<unsigned char> load_font_file(char const *path) {
  std::ifstream f(path, std::ios::binary | std::ios::ate);
  if(!f) return {};
  auto const end{f.tellg()};
  if(end <= 0) return {};
  size_t const size{static_cast<size_t>(end)};
  f.seekg(0, std::ios::beg);
  if(!f) return {};
  std::vector<unsigned char> buf(size);
  if(!f.read(reinterpret_cast<char *>(buf.data()), static_cast<std::streamsize>(size))) return {};
  return buf;
}

// ---------------------------------------------------------------------------
// aligntype
// ---------------------------------------------------------------------------
TEST_CASE("aligntype enum has expected values", "[aligntype]") {
  using vectorfontstorm::aligntype;

  // Verify all nine values are distinct
  CHECK(aligntype::TOPLEFT    != aligntype::TOP);
  CHECK(aligntype::TOP        != aligntype::TOPRIGHT);
  CHECK(aligntype::LEFT       != aligntype::CENTRE);
  CHECK(aligntype::RIGHT      != aligntype::CENTRE);
  CHECK(aligntype::BOTTOMLEFT != aligntype::BOTTOM);
  CHECK(aligntype::BOTTOM     != aligntype::BOTTOMRIGHT);

  // Verify they can be used in a switch without compile error
  aligntype a{aligntype::CENTRE};
  bool reached{false};
  switch(a) {
  case aligntype::TOPLEFT:    break;
  case aligntype::TOP:        break;
  case aligntype::TOPRIGHT:   break;
  case aligntype::LEFT:       break;
  case aligntype::RIGHT:      break;
  case aligntype::CENTRE:     reached = true; break;
  case aligntype::BOTTOMLEFT: break;
  case aligntype::BOTTOM:     break;
  case aligntype::BOTTOMRIGHT:break;
  }
  CHECK(reached);
}

// ---------------------------------------------------------------------------
// buffer_data
// ---------------------------------------------------------------------------
TEST_CASE("buffer_data default construction yields empty containers", "[buffer_data]") {
  vectorfontstorm::buffer_data<test_vertex> bd;
  CHECK(bd.vbo.empty());
  CHECK(bd.ibo.empty());
}

TEST_CASE("buffer_data can store vertices and indices", "[buffer_data]") {
  vectorfontstorm::buffer_data<test_vertex> bd;

  bd.vbo.emplace_back(1.0f, 2.0f, 3.0f);
  bd.vbo.emplace_back(4.0f, 5.0f, 6.0f);
  bd.ibo.push_back(0);
  bd.ibo.push_back(1);

  REQUIRE(bd.vbo.size() == 2);
  REQUIRE(bd.ibo.size() == 2);
  CHECK(bd.vbo[0].x == 1.0f);
  CHECK(bd.vbo[0].y == 2.0f);
  CHECK(bd.vbo[0].z == 3.0f);
  CHECK(bd.ibo[0] == 0);
  CHECK(bd.ibo[1] == 1);
}

TEST_CASE("buffer_data is independently copyable", "[buffer_data]") {
  vectorfontstorm::buffer_data<test_vertex> src;
  src.vbo.emplace_back(0.0f, 1.0f, 2.0f);
  src.ibo.push_back(0);

  auto dst{src};   // copy
  REQUIRE(dst.vbo.size() == 1);
  REQUIRE(dst.ibo.size() == 1);

  // Modifying src does not affect dst
  src.vbo.emplace_back(9.0f, 9.0f, 9.0f);
  CHECK(dst.vbo.size() == 1);
}

// ---------------------------------------------------------------------------
// glyph – direct unit tests (geometry is exercised via font below)
// ---------------------------------------------------------------------------
TEST_CASE("glyph get_advance returns the value given at construction", "[glyph]") {
  vectorfontstorm::glyph<test_vertex> g(U'A', 0.75f);
  CHECK(g.get_advance() == Catch::Approx(0.75f));
}

TEST_CASE("glyph with zero advance", "[glyph]") {
  vectorfontstorm::glyph<test_vertex> g(U' ', 0.0f);
  CHECK(g.get_advance() == Catch::Approx(0.0f));
}

TEST_CASE("glyph correct_winding marks empty glyph as whitespace", "[glyph]") {
  vectorfontstorm::glyph<test_vertex> g(U'X', 0.5f);
  // No contours have been added; correct_winding should detect this and set
  // the internal whitespace flag.  After that, all get_* methods should
  // return nothing.
  g.correct_winding();
  // Calling correct_winding a second time exercises the early-return path
  // that skips processing when whitespace is already set.
  g.correct_winding();

  vectorfontstorm::buffer_data<test_vertex> data;
  g.get_outline(data);
  g.get_fill(data);
  g.get_back(data);
  g.get_edge(data);

  CHECK(data.vbo.empty());
  CHECK(data.ibo.empty());
}

TEST_CASE("glyph cache_buffer on empty glyph leaves data empty", "[glyph]") {
  vectorfontstorm::glyph<test_vertex> g(U'Y', 0.4f);
  g.cache_buffer();

  vectorfontstorm::buffer_data<test_vertex> out;
  g.get_fill(out);
  CHECK(out.vbo.empty());
}

TEST_CASE("glyph get_buffer copies data with correct index offsets", "[glyph]") {
  // get_buffer is public and can be exercised directly with pre-populated
  // data_in / data_out – this tests the index-offset logic.
  vectorfontstorm::glyph<test_vertex> g(U'A', 0.5f);

  vectorfontstorm::buffer_data<test_vertex> src;
  src.vbo.emplace_back(0.1f, 0.2f, 0.0f);
  src.vbo.emplace_back(0.3f, 0.4f, 0.0f);
  src.vbo.emplace_back(0.5f, 0.6f, 0.0f);
  src.ibo.push_back(0);
  src.ibo.push_back(1);
  src.ibo.push_back(2);

  vectorfontstorm::buffer_data<test_vertex> dst;
  // Pre-populate dst with two existing vertices so the offset matters
  dst.vbo.emplace_back(10.0f, 10.0f, 0.0f);
  dst.vbo.emplace_back(11.0f, 11.0f, 0.0f);
  dst.ibo.push_back(0);
  dst.ibo.push_back(1);

  g.get_buffer(src, dst);

  // dst should now have 5 vertices (2 original + 3 new)
  REQUIRE(dst.vbo.size() == 5);
  // Indices for the new triangle must be offset by 2
  REQUIRE(dst.ibo.size() == 5);
  CHECK(dst.ibo[2] == 2);   // 0 + offset 2
  CHECK(dst.ibo[3] == 3);   // 1 + offset 2
  CHECK(dst.ibo[4] == 4);   // 2 + offset 2
}

// ---------------------------------------------------------------------------
// font integration tests – require a real TTF file.
// ---------------------------------------------------------------------------

#ifdef TEST_FONT_PATH
#define FONT_PATH_DEFINED 1
#else
#define FONT_PATH_DEFINED 0
#define TEST_FONT_PATH ""
#endif

/// RAII wrapper that owns both the raw font bytes and the font object.
/// Both must be kept alive together: font<T> stores a pointer into the byte
/// buffer passed to its constructor, so the buffer must outlive the font.
struct font_fixture {
  std::vector<unsigned char> bytes;
  std::unique_ptr<vectorfontstorm::font<test_vertex>> f;

  explicit font_fixture(double height = 1.0) {
    bytes = load_font_file(TEST_FONT_PATH);
    REQUIRE_FALSE(bytes.empty());
    f = std::make_unique<vectorfontstorm::font<test_vertex>>(bytes.data(), bytes.size(), height);
  }
  vectorfontstorm::font<test_vertex> &font() { return *f; }
};

TEST_CASE("font: height", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available (set TEST_FONT_PATH)");
  }
  SECTION("default height is 1.0") {
    std::vector<unsigned char> bytes{load_font_file(TEST_FONT_PATH)};
    REQUIRE_FALSE(bytes.empty());
    vectorfontstorm::font<test_vertex> f(bytes.data(), bytes.size());
    CHECK(f.get_height() == Catch::Approx(1.0));
  }
  SECTION("configured height is returned") {
    font_fixture fx(2.5);
    CHECK(fx.font().get_height() == Catch::Approx(2.5));
  }
}

TEST_CASE("font: get_outline produces non-empty line data for printable chars", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  for(char32_t ch : std::u32string{U'A', U'B', U'H', U'I', U'Z', U'1', U'8'}) {
    vectorfontstorm::buffer_data<test_vertex> data;
    float const advance{fx.font().get_outline(ch, data)};
    INFO("Character: " << static_cast<char>(ch));
    CHECK(advance > 0.0f);
    CHECK_FALSE(data.vbo.empty());
    CHECK_FALSE(data.ibo.empty());
    // Outline stores pairs of line endpoints – must be even
    CHECK(data.ibo.size() % 2 == 0);
  }
}

TEST_CASE("font: get_fill produces triangulated front face", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  for(char32_t ch : std::u32string{U'A', U'O', U'B', U'8'}) {
    vectorfontstorm::buffer_data<test_vertex> data;
    float const advance{fx.font().get_fill(ch, data)};
    INFO("Character: " << static_cast<char>(ch));
    CHECK(advance > 0.0f);
    CHECK_FALSE(data.vbo.empty());
    // fill is triangles – index count must be a multiple of 3
    CHECK(data.ibo.size() % 3 == 0);
    // Every index must be a valid vertex index
    for(auto idx : data.ibo) {
      CHECK(idx < data.vbo.size());
    }
  }
}

TEST_CASE("font: get_back mirrors fill geometry", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> fill, back;
  fx.font().get_fill(U'A', fill);
  fx.font().get_back(U'A', back);

  // Back face should have the same vertex/index count as the front face
  CHECK(fill.vbo.size() == back.vbo.size());
  CHECK(fill.ibo.size() == back.ibo.size());

  // Front face vertices have z=0, back face vertices have z=-1
  for(auto const &v : fill.vbo) {
    CHECK(v.z == Catch::Approx(0.0f));
  }
  for(auto const &v : back.vbo) {
    CHECK(v.z == Catch::Approx(-1.0f));
  }
}

TEST_CASE("font: get_edge produces extruded side geometry", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> edge;
  float const advance{fx.font().get_edge(U'A', edge)};
  CHECK(advance > 0.0f);
  CHECK_FALSE(edge.vbo.empty());
  // Edge quads are stored as triangle pairs (6 verts each) – size % 6 == 0
  CHECK(edge.ibo.size() % 6 == 0);
}

TEST_CASE("font: whitespace characters produce no geometry but positive advance", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  for(char32_t ch : std::u32string{U' '}) {
    vectorfontstorm::buffer_data<test_vertex> data;
    float const advance{fx.font().get_outline(ch, data)};
    INFO("Character: " << static_cast<char>(ch));
    CHECK(advance > 0.0f);
    CHECK(data.vbo.empty());
  }
}

TEST_CASE("font: glyph caching returns identical data on repeated calls", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> first, second;
  fx.font().get_fill(U'G', first);
  fx.font().get_fill(U'G', second);

  REQUIRE(first.vbo.size() == second.vbo.size());
  REQUIRE(first.ibo.size() == second.ibo.size());
  for(size_t i{0}; i != first.vbo.size(); ++i) {
    CHECK(first.vbo[i].x == Catch::Approx(second.vbo[i].x));
    CHECK(first.vbo[i].y == Catch::Approx(second.vbo[i].y));
    CHECK(first.vbo[i].z == Catch::Approx(second.vbo[i].z));
  }
  for(size_t i{0}; i != first.ibo.size(); ++i) {
    CHECK(first.ibo[i] == second.ibo[i]);
  }
}

TEST_CASE("font: combined get_outline_and_fill populates all three buffers", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> outline, fill, back;
  float const advance{fx.font().get_outline_and_fill(U'E', outline, fill, back)};

  CHECK(advance > 0.0f);
  CHECK_FALSE(outline.vbo.empty());
  CHECK_FALSE(fill.vbo.empty());
  CHECK_FALSE(back.vbo.empty());
  CHECK_FALSE(outline.ibo.empty());
  CHECK_FALSE(fill.ibo.empty());
  CHECK_FALSE(back.ibo.empty());
  // Outline indices form line segments; fill/back indices form triangles.
  CHECK(outline.ibo.size() % 2 == 0);
  CHECK(fill.ibo.size() % 3 == 0);
  CHECK(back.ibo.size() % 3 == 0);
}

TEST_CASE("font: combined get_outline_and_fill_and_edges populates all four buffers", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> outline, fill, back, edge;
  float const advance{fx.font().get_outline_and_fill_and_edges(U'N', outline, fill, back, edge)};

  CHECK(advance > 0.0f);
  CHECK_FALSE(outline.vbo.empty());
  CHECK_FALSE(fill.vbo.empty());
  CHECK_FALSE(back.vbo.empty());
  CHECK_FALSE(edge.vbo.empty());
}

TEST_CASE("font: different characters have different advances", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> d;
  float const adv_i{fx.font().get_outline(U'I', d)};
  d = {};
  float const adv_w{fx.font().get_outline(U'W', d)};

  // 'W' is wider than 'I' in almost every font
  CHECK(adv_w > adv_i);
}

TEST_CASE("font: vertices are within expected coordinate range for height=1", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx(1.0);

  vectorfontstorm::buffer_data<test_vertex> data;
  fx.font().get_fill(U'A', data);
  REQUIRE_FALSE(data.vbo.empty());

  // At height=1 the tallest glyph geometry should be at most ~1 unit tall
  // (some descenders may push below 0).  Use a generous but finite bound.
  for(auto const &v : data.vbo) {
    CHECK(v.x > -2.0f);
    CHECK(v.x <  2.0f);
    CHECK(v.y > -2.0f);
    CHECK(v.y <  2.0f);
  }
}

TEST_CASE("font: height scaling produces proportional coordinates", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }

  std::vector<unsigned char> bytes{load_font_file(TEST_FONT_PATH)};
  REQUIRE_FALSE(bytes.empty());
  vectorfontstorm::font<test_vertex> f1(bytes.data(), bytes.size(), 1.0);
  vectorfontstorm::font<test_vertex> f2(bytes.data(), bytes.size(), 2.0);

  vectorfontstorm::buffer_data<test_vertex> d1, d2;
  f1.get_fill(U'H', d1);
  f2.get_fill(U'H', d2);

  REQUIRE(d1.vbo.size() == d2.vbo.size());
  // Every vertex in the 2× font should be approximately twice the coordinate
  for(size_t i{0}; i != d1.vbo.size(); ++i) {
    CHECK(d2.vbo[i].x == Catch::Approx(d1.vbo[i].x * 2.0f).margin(1e-4));
    CHECK(d2.vbo[i].y == Catch::Approx(d1.vbo[i].y * 2.0f).margin(1e-4));
  }
}

TEST_CASE("font: unicode characters produce geometry", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  // DejaVuSans covers the Latin-1 supplement block
  for(char32_t ch : std::u32string{U'\u00C9', U'\u00D1', U'\u00DC'}) { // É Ñ Ü
    vectorfontstorm::buffer_data<test_vertex> data;
    float const advance{fx.font().get_fill(ch, data)};
    INFO("Codepoint: U+" << std::hex << static_cast<unsigned>(ch));
    CHECK(advance > 0.0f);
    CHECK_FALSE(data.vbo.empty());
  }
}

TEST_CASE("font: glyph with two contours (letter with hole) triangulates correctly", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  // 'O' has an outer contour and an inner hole
  vectorfontstorm::buffer_data<test_vertex> fill;
  fx.font().get_fill(U'O', fill);

  REQUIRE_FALSE(fill.vbo.empty());
  // Triangulation must produce triangles
  CHECK(fill.ibo.size() % 3 == 0);
  // All indices must be valid
  for(auto idx : fill.ibo) {
    CHECK(idx < fill.vbo.size());
  }
}

TEST_CASE("font: front-fill and back-fill have opposite z-coordinates", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  vectorfontstorm::buffer_data<test_vertex> fill, back;
  fx.font().get_fill(U'M', fill);
  fx.font().get_back(U'M', back);

  // Both buffers must be the same size (back face is the same triangulation
  // but with reversed winding order, so vertex ordering within triangles differs).
  REQUIRE(fill.vbo.size() == back.vbo.size());
  REQUIRE(fill.ibo.size() == back.ibo.size());

  // All front-face vertices have z=0; all back-face vertices have z=-1.
  for(auto const &v : fill.vbo) {
    CHECK(v.z == Catch::Approx(0.0f));
  }
  for(auto const &v : back.vbo) {
    CHECK(v.z == Catch::Approx(-1.0f));
  }
}

TEST_CASE("font: swap correctly exchanges two font objects", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  std::vector<unsigned char> bytes{load_font_file(TEST_FONT_PATH)};
  REQUIRE_FALSE(bytes.empty());

  vectorfontstorm::font<test_vertex> fa(bytes.data(), bytes.size(), 1.0);
  vectorfontstorm::font<test_vertex> fb(bytes.data(), bytes.size(), 3.0);

  CHECK(fa.get_height() == Catch::Approx(1.0));
  CHECK(fb.get_height() == Catch::Approx(3.0));

  fa.swap(fb);

  CHECK(fa.get_height() == Catch::Approx(3.0));
  CHECK(fb.get_height() == Catch::Approx(1.0));
}

TEST_CASE("font: lowercase characters produce outline geometry", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  // Lowercase letters with curves are common in TrueType fonts and exercise
  // contour paths not covered by uppercase-only tests.  Use get_outline (no
  // triangulation) to avoid poly2tri edge cases on degenerate contours.
  for(char32_t ch : std::u32string{U'a', U'b', U'c', U'd', U'e', U'f', U'g',
                                    U'h', U'i', U'j', U'k', U'l', U'm', U'n',
                                    U'o', U'p', U'q', U'r', U's', U't', U'u',
                                    U'v', U'w', U'x', U'y', U'z'}) {
    vectorfontstorm::buffer_data<test_vertex> data;
    float const advance{fx.font().get_outline(ch, data)};
    INFO("Character: " << static_cast<char>(ch));
    CHECK(advance > 0.0f);
  }
}

TEST_CASE("font: all four buffer types for a curved character", "[font][integration]") {
  if(!FONT_PATH_DEFINED || std::string(TEST_FONT_PATH).empty()) {
    SKIP("No test font available");
  }
  font_fixture fx;

  // Exercise get_outline, get_fill, get_back and get_edge for characters that
  // have both straight and curved segments.
  for(char32_t ch : std::u32string{U'a', U'e', U'o', U's', U'c'}) {
    vectorfontstorm::buffer_data<test_vertex> outline, fill, back, edge;
    float const adv{fx.font().get_outline_and_fill_and_edges(ch, outline, fill, back, edge)};
    INFO("Character: " << static_cast<char>(ch));
    CHECK(adv > 0.0f);
    CHECK_FALSE(outline.vbo.empty());
    CHECK_FALSE(fill.vbo.empty());
    CHECK_FALSE(back.vbo.empty());
    CHECK_FALSE(edge.vbo.empty());
  }
}
