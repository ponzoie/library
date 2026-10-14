# 競技プログラミング用ライブラリ

ABC の F 程度まででよく使う道具を、[AtCoder Library（ACL）](https://atcoder.github.io/ac-library/master/document_ja/) に追加する C++20 のヘッダーライブラリです。名前空間は `ponzoie`。ACL にある機能は ACL を利用します。

```cpp
#include <atcoder/modint>
#include <ponzoie/dual_segtree>
#include <ponzoie/interval_set>
#include <ponzoie/matrix>
```

よくある実装を再利用し、問題ごとの考察と組み合わせに集中するための構成です。すべての F 問題をライブラリだけで解けるという意味ではありません。

## 使い始める

このリポジトリと `ac-library` を隣に置く場合:

```text
cpp/
├── ac-library/
│   └── atcoder/
└── library/
    ├── ponzoie/
    ├── template.cpp
    └── tools/
```

`library` ディレクトリに解答の `main.cpp` を置き、次でビルド・実行します。初めてなら `template.cpp` をコピーして解答を書き始められます。

```bash
cp template.cpp main.cpp  # 初回のみ。既存の解答がある場合は不要
make run
```

**毎回、自作ヘッダーの展開 → ビルド → 実行**を行います。ビルドだけなら `make build`、別の解答なら `make run SOURCE=abc.cpp`。解答の隣に提出用の `submit.cpp` と実行ファイル `build/解答名` が生成されます。VS Code では付属のタスクで同じ処理を実行できます。

必要な機能だけを `#include <ponzoie/機能名>` で読み込めます。一括読み込みは `#include <ponzoie/all>`。`.hpp` 付きでも利用できます。雛形は [template.cpp](template.cpp)。自動ビルド・VS Code の使い方は [環境・提出の手順](docs/setup.md) を参照してください。

## AtCoder に提出する

`make build` / `make run` または VS Code のタスクを使うと、**ビルド時に提出用の `submit.cpp` も自動生成**されます。その内容を丸ごと提出欄に貼り付けます。手元の解答は include 形式のままで構いません。標準ライブラリと `<atcoder/...>` の include は維持します。

make を使わず直接実行する場合:

```bash
python3 tools/build.py main.cpp --run
```

コンパイルするのは生成した `submit.cpp` 自体です。ヘッダーだけ変更した場合も次のビルドで反映され、ビルド失敗時には実行しません。提出時は C++20 以降に対応する言語設定を選びます。手動で展開だけを行う方法などは [提出の手順](docs/setup.md) を参照してください。

## ライブラリ一覧

各ページに API、制約、計算量、使用例があります。添字は原則 **0-indexed**、区間は **半開区間 `[l, r)`** です。

### データ構造

| include | 使いどころ | リファレンス |
| --- | --- | --- |
| `<ponzoie/dual_segtree>` | 区間更新・一点取得。加算、代入、アフィン変換の型も用意 | [双対セグメント木](docs/dual_segtree.md) |
| `<ponzoie/interval_set>` | 整数区間の追加・削除・mex・被覆長を管理 | [区間集合](docs/interval_set.md) |
| `<ponzoie/interval_map>` | 同じ値が続く区間をまとめて管理、区間の色を塗り替え | [区間マップ](docs/interval_map.md) |
| `<ponzoie/weighted_dsu>` | 差分制約と矛盾検出 | [重み付き Union-Find](docs/weighted_dsu.md) |
| `<ponzoie/sparse_table>` | 更新のない区間 min / max / gcd | [Sparse Table](docs/sparse_table.md) |
| `<ponzoie/compress>` | 座標圧縮と元の値への復元 | [座標圧縮](docs/compress.md) |
| `<ponzoie/binary_trie>` | 整数多重集合の k 番目・最小/最大 XOR | [Binary Trie](docs/binary_trie.md) |

### グラフ・木

| include | 使いどころ | リファレンス |
| --- | --- | --- |
| `<ponzoie/graph>` | 非重み付き/重み付きグラフの共通の型 | [グラフの型](docs/graph.md) |
| `<ponzoie/shortest_path>` | BFS・0-1 BFS・Dijkstra・Bellman-Ford・Floyd-Warshall、経路復元 | [最短路](docs/shortest_path.md) |
| `<ponzoie/topological_sort>` | DAG の順序、閉路の検出 | [トポロジカルソート](docs/topological_sort.md) |
| `<ponzoie/lca>` | LCA、木上距離、k 個先の頂点、部分木の区間 | [LCA](docs/lca.md) |
| `<ponzoie/lowlink>` | 橋・関節点。多重辺・自己ループ対応 | [Lowlink](docs/lowlink.md) |
| `<ponzoie/tree_diameter>` | 非重み付き木の直径と経路 | [木の直径](docs/tree_diameter.md) |
| `<ponzoie/rerooting>` | 全頂点を根にした木 DP。距離総和など | [全方位木 DP](docs/rerooting.md) |

### 数学・数列・文字列

| include | 使いどころ | リファレンス |
| --- | --- | --- |
| `<ponzoie/matrix>` | 行列演算・累乗、線形漸化式。ACL modint と併用 | [行列](docs/matrix.md) |
| `<ponzoie/prime>` | 素数表・素因数分解・約数列挙・Euler の φ 関数 | [素数・約数](docs/prime.md) |
| `<ponzoie/combination>` | 階乗・逆階乗・二項係数・順列・重複組合せ | [組合せ](docs/combination.md) |
| `<ponzoie/integer>` | 負数対応の切り捨て/切り上げ除算、整数平方根 | [整数演算](docs/integer.md) |
| `<ponzoie/xor_basis>` | XOR 線形基底、生成できる XOR の判定・最大値 | [XOR 基底](docs/xor_basis.md) |
| `<ponzoie/prefix_sum>` | 1 次元・2 次元累積和 | [累積和](docs/prefix_sum.md) |
| `<ponzoie/sequence>` | LIS 復元・転倒数・ランレングス圧縮 | [数列](docs/sequence.md) |
| `<ponzoie/trie>` | 文字列多重集合・共通接頭辞の個数・削除 | [Trie](docs/trie.md) |
| `<ponzoie/utility>` | chmin / chmax、整数二分探索 | [小さな補助関数](docs/utility.md) |

使い分けや問題ごとの実装の出発点は [レシピ集](docs/recipes.md)。実行可能な短いプログラムは [examples](examples/) にあります。各機能を試せる公開問題は [確認問題一覧](docs/verification.md) にまとめています。

## ACL を使うもの

通常の Union-Find / Fenwick Tree / Segtree / Lazy Segtree / Modint / Convolution / SCC / 2-SAT / Max Flow / Min Cost Flow / Suffix Array / LCP / Z Algorithm / CRT / floor_sum などは、[ACL 公式ドキュメント](https://atcoder.github.io/ac-library/master/document_ja/) を参照してください。二重に実装しません。`sequence` の転倒数にも ACL の Fenwick Tree を利用しています。

双対セグメント木は「区間更新・一点取得」に向いています。区間更新後に**区間和や区間最大値**を取りたい場合は ACL の `lazy_segtree` が必要です。[レシピ集](docs/recipes.md) に選び方をまとめています。

## 共通の制約

- GCC / Clang 系の C++20 を想定しています。一部に `__int128` を使います。
- 各 API の制約は呼び出し側が守ります。`assert` は開発時の補助であり、入力検証を保証するものではありません。
- 明記していない限り、整数オーバーフローを自動で防ぎません。値・中間計算・要素数が利用する型に収まる必要があります。
- グラフの DFS 相当の処理と全方位木 DP は非再帰です。木の API は連結な無向木が前提です。
- 複雑度は、利用者が渡す演算と値のコピーを O(1) とした場合です。

## 検証

```bash
python3 tools/test.py --acl-dir ../ac-library
python3 tools/test.py --acl-dir ../ac-library --sanitize
```

固定 seed のランダム差分テスト、境界値、20 万頂点の鎖、全ヘッダーの単独/重複 include、複数翻訳単位でのリンク、使用例の実行、提出用コードのビルドを確認します。公開問題に対応する [verify](verify/) の解答も展開して、保存した公式サンプル・自作ケースと照合します。GitHub Actions でも同じコマンドを実行します。これはローカル検証であり、AtCoder などでの AC 実績を表すものではありません。

公開問題形式の検証だけを実行する場合:

```bash
python3 tools/verify.py --acl-dir ../ac-library
```

テストの内訳と、各機能に対応する問題・注意点は [確認問題一覧](docs/verification.md) を参照してください。

## 以前のコードからの変更

ACL と重複する `BIT.hpp` / `UnionFind.hpp` / `segmenttree.hpp` / `pow.hpp` と、旧 `DualSegmentTree.hpp` / `Trietree.hpp` は整理しました。対応する ACL または新 API に移行してください。行列の `math.hpp` は互換 include として残し、`Matrix` / `Ring` / `at(i,j)` / `identity_e(n)` も維持しています。

ドキュメントの構成は ACL の「API・制約・計算量・使用例」を参考にし、本文と追加実装はこのリポジトリ用に作成しています。

## ライセンス

MIT License。詳細は [LICENSE](LICENSE) を参照してください。
