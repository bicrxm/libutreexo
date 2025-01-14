#ifndef UTREEXO_POLLARD_H
#define UTREEXO_POLLARD_H

#include <optional>

#include "accumulator.h"

namespace utreexo {

class Pollard : public Accumulator
{
private:
    class InternalNode;
    /* Pollards implementation of Accumulator::Node */
    class Node;

    NodePtr<Pollard::InternalNode> m_remember;

    /*
     * Return the node and its sibling.
     * Point path to the parent of the node. The path to the node can be traversed in reverse order using the
     * Accumulator::Node::Parent function.
     */
    std::vector<NodePtr<Pollard::InternalNode>> Read(uint64_t pos, NodePtr<Accumulator::Node>& path, bool record_path) const;

    std::optional<const Hash> Read(uint64_t pos) const override;
    std::vector<Hash> ReadLeafRange(uint64_t pos, uint64_t range) const override;
    NodePtr<Accumulator::Node> SwapSubTrees(uint64_t from, uint64_t to) override;
    NodePtr<Accumulator::Node> MergeRoot(uint64_t parent_pos, Hash parent_hash) override;
    NodePtr<Accumulator::Node> NewLeaf(const Leaf& hash) override;
    void FinalizeRemove(uint64_t next_num_leaves) override;

    void InitChildrenOfComputed(NodePtr<Pollard::Node>& node,
                                NodePtr<Pollard::Node>& left_child,
                                NodePtr<Pollard::Node>& right_child,
                                bool& recover_left,
                                bool& recover_right);

    bool CreateProofTree(std::vector<NodePtr<Node>>& proof_tree,
                         std::vector<std::pair<NodePtr<Node>, int>>& recovery,
                         const BatchProof& proof);

    bool VerifyProofTree(std::vector<NodePtr<Pollard::Node>> proof_tree,
                         const std::vector<Hash>& target_hashes,
                         const std::vector<Hash>& proof_hashes);

public:
    Pollard(const std::vector<Hash>& roots, uint64_t num_leaves);
    Pollard(uint64_t num_leaves);
    ~Pollard();

    bool Verify(const BatchProof& proof, const std::vector<Hash>& target_hashes) override;

    /** Prune everything except the roots. */
    void Prune();

    uint64_t NumCachedLeaves() const { return m_remember ? m_remember.use_count() - 1 : 0; }
};

};     // namespace utreexo
#endif // UTREEXO_POLLARD_H
