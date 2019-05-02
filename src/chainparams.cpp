// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017 The ONEX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "masternode.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector <CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count) {
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0, uint256("0000eaf02557cabd4241d4e7df59d2479bd746f547f8be1252e31d672a9b83b0"))
        (1000, uint256("000000022c8564112392c20e422dc3fafeb1839f656219ee6b50e09323706f9f"));


static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1556758437, // * UNIX timestamp of last checkpoint block
        1220,          // * total number of transactions between genesis and last checkpoint
        //   (the tx=... number in the SetBestChain debug.log lines)
        2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1740710,
        0,
        250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        1454124731,
        0,
        100};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x26;
        pchMessageStart[1] = 0xf3;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0xa3;
        vAlertPubKey = ParseHex(
                "041ff106bb9cfb2eafbdc7a52803f6133e0ff4db6e3407893cec8fb841fc0b6532e675e42ea80566a798ee891a64535bf98991c174f9d4ae78eca6e807214c38af");
        nDefaultPort = 30229;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nSubsidyHalvingInterval = 1050000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 10 * 60; // ONEX:
        nTargetSpacing = 1 * 60;  // ONEX:
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 100000000 * COIN;
        nStakeInputMinimal = 100 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 1000;
        nModifierUpdateBlock = 1;

        const char *pszTimestamp = "Don't work for weekends, work for our goals - 9/Apr/2019.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
                                           << vector<unsigned char>((const unsigned char *) pszTimestamp,
                                                                    (const unsigned char *) pszTimestamp +
                                                                    strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex(
                "04f5eaf3186ae430ecb2e602e1cea07abd57def84271787b0692bb9eee62367be9a79251575574a2051e4f9e0bc264b3f7af9143241fc4e6f854a4162320228f9e")
                                               << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1555862289;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 0xa0d6;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0000eaf02557cabd4241d4e7df59d2479bd746f547f8be1252e31d672a9b83b0"));
        assert(genesis.hashMerkleRoot == uint256("434f9e95f971b5a83536b009c780b9e923bc845e3a4b37b18756bc75f9823c60"));


        // DNS Seeding
        vSeeds.push_back(CDNSSeedData("seed1", "node1.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed2", "node2.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed3", "node3.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed4", "node4.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed5", "node5.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed6", "node6.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed7", "node7.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed8", "node8.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed9", "node9.oneexbit.com"));
        vSeeds.push_back(CDNSSeedData("seed10", "node10.oneexbit.com"));

        // ONEX addresses start with 'G'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 50);
        // ONEX script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 110);
        // ONEX private keys start with 'K'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 105);
        // ONEX BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] =
                boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container < std::vector < unsigned char > >
                ();
        // ONEX BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] =
                boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container < std::vector < unsigned char > >
                ();
        // ONEX BIP44 coin type is '222' (0x800000de)
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] =
                boost::assign::list_of(0x80)(0x00)(0x00)(0xde).convert_to_container < std::vector < unsigned char > >
                ();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));


        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 2;
        strSporkKey = "046e31a5f744cac510a962a7d755028a15bea275993a9ac392997268c01151aee66a0334ab2e07649dca87cd2f5a567da3f91484338d14b032c22170d9ce3a67e8";
        strMasternodePoolDummyAddress = "MAKzgFX8EZt3fwzkPv55Q3sZJauPNdR8DQ";
        nStartMasternodePayments = genesis.nTime;

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData &Checkpoints() const {
        return data;
    }
};

static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x4a;
        pchMessageStart[1] = 0x2d;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        vAlertPubKey = ParseHex(
                "041b2b4c86273359acac3522471911ed2b303eaab65e8a1de01c06e89f2eab1e55234a4b504f3ce20c6f661f007d0ca15623b4358d9855c7c8ba793a24fa315e22");
        nDefaultPort = 130229;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // ONEX: 1 day
        nTargetSpacing = 2 * 60;  // ONEX: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 21000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1516926684;
        genesis.nNonce = 3097091;

        hashGenesisBlock = genesis.GetHash();
        // assert(hashGenesisBlock == uint256("0x00000ec1634e6ee176e84d803713f1dff191abbe0add7a59b817fe56db094b76"));

        vFixedSeeds.clear();
        vSeeds.clear();

        //  vSeeds.push_back(CDNSSeedData("159.89.119.230", "159.89.119.230"));

        // Testnet ONEX addresses start with 'g'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 98);
        // Testnet ONEX script addresses start with '5' or '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);
        // Testnet private keys start with 'k'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 108);
        // Testnet ONEX BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] =
                boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container < std::vector < unsigned char > >
                ();
        // Testnet ONEX BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] =
                boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container < std::vector < unsigned char > >
                ();
        // Testnet bitcoin green BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] =
                boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container < std::vector < unsigned char > >
                ();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0465acee8612d5d441ad77a835c132aee2eeec9fa530c3b9ec3f24b8e46de9642b9064008268ed83cdb6498d4e1b1b89e51dc41504e26568e8b6dcb59f82552a9c";
        strMasternodePoolDummyAddress = "pbJ4Qad4xc77PpLzMx6rUegAs6aUPWkcUq";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
        // here because we only have a 8 block finalization window on testnet
    }

    const Checkpoints::CCheckpointData &Checkpoints() const {
        return dataTestnet;
    }
};

static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x20;
        pchMessageStart[1] = 0xee;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // ONEX: 1 day
        nTargetSpacing = 2 * 60;        // ONEX: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1516926684;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 230229;
        // assert(hashGenesisBlock == uint256("0x40d358c1e5dfd45b317f749c5aeeee6842c49fcb9db024e1a52b54ba7f7b1fff"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData &Checkpoints() const {
        return dataRegtest;
    }
};

static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData &Checkpoints() const {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void
    setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }

    virtual void setEnforceBlockUpgradeMajority(
            int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }

    virtual void setRejectBlockOutdatedMajority(
            int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }

    virtual void setToCheckBlockUpgradeMajority(
            int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }

    virtual void setDefaultConsistencyChecks(
            bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }

    virtual void setAllowMinDifficultyBlocks(
            bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }

    virtual void
    setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams() {
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams *) &unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine() {
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

