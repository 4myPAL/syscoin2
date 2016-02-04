#ifndef OFFER_H
#define OFFER_H

#include "rpcserver.h"
#include "dbwrapper.h"
#include "script/script.h"
#include "serialize.h"
class CWalletTx;
class CTransaction;
class CReserveKey;
class CValidationState;
class CCoinsViewCache;
class CCoins;
class CBlockIndex;

bool CheckOfferInputs(const CTransaction &tx, CValidationState &state, const CCoinsViewCache &inputs, bool fBlock, bool fMiner, bool fJustCheck, int nHeight, bool fRescan = false);


bool DecodeOfferTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeAndParseOfferTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeOfferScript(const CScript& script, int& op, std::vector<std::vector<unsigned char> > &vvch);
bool IsOfferOp(int op);
int IndexOfOfferOutput(const CTransaction& tx);
int GetOfferExpirationDepth();
std::string offerFromOp(int op);
CScript RemoveOfferScriptPrefix(const CScript& scriptIn);
void rescanforsyscoinservices(CBlockIndex *pindexRescan);
static const char* norefund = "norefund";
static const char* inprogress = "inprogress";
static const char* complete = "complete";
const std::vector<unsigned char> OFFER_NOREFUND = std::vector<unsigned char>(norefund, norefund + strlen(norefund));
static const std::vector<unsigned char> OFFER_REFUND_PAYMENT_INPROGRESS = std::vector<unsigned char>(inprogress, inprogress + strlen(inprogress));
static const std::vector<unsigned char> OFFER_REFUND_COMPLETE = std::vector<unsigned char>(complete, complete + strlen(complete));

class COfferAccept {
public:
	std::vector<unsigned char> vchAcceptRand;
    std::vector<unsigned char> vchMessage;
	uint256 txHash;
	uint64_t nHeight;
	unsigned int nQty;
	float nPrice;
	bool bRefunded;
	uint256 txBTCId;
	std::vector<unsigned char> vchLinkOfferAccept;
	std::vector<unsigned char> vchLinkOffer;
	std::vector<unsigned char> vchBuyerKey;	
	COfferAccept() {
        SetNull();
    }

	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
		READWRITE(vchAcceptRand);
        READWRITE(vchMessage);
		READWRITE(txHash);
		READWRITE(VARINT(nHeight));
        READWRITE(VARINT(nQty));
    	READWRITE(nPrice);
		READWRITE(bRefunded);
		READWRITE(vchLinkOfferAccept);
		READWRITE(vchBuyerKey);	
		READWRITE(vchLinkOffer);
		READWRITE(txBTCId);	
		
	}

    friend bool operator==(const COfferAccept &a, const COfferAccept &b) {
        return (
		a.vchAcceptRand == b.vchAcceptRand
        && a.vchMessage == b.vchMessage
        && a.txHash == b.txHash
        && a.nHeight == b.nHeight
        && a.nQty == b.nQty
        && a.nPrice == b.nPrice
		&& a.bRefunded == b.bRefunded
		&& a.vchLinkOfferAccept == b.vchLinkOfferAccept
		&& a.vchBuyerKey == b.vchBuyerKey
		&& a.vchLinkOffer == b.vchLinkOffer 
		&& a.txBTCId == b.txBTCId
        );
    }

    COfferAccept operator=(const COfferAccept &b) {
		vchAcceptRand = b.vchAcceptRand;
        vchMessage = b.vchMessage;
        txHash = b.txHash;
        nHeight = b.nHeight;
        nQty = b.nQty;
        nPrice = b.nPrice;
		bRefunded = b.bRefunded;
		vchLinkOfferAccept = b.vchLinkOfferAccept;
		vchBuyerKey = b.vchBuyerKey;
		vchLinkOffer = b.vchLinkOffer;
		txBTCId = b.txBTCId;
        return *this;
    }

    friend bool operator!=(const COfferAccept &a, const COfferAccept &b) {
        return !(a == b);
    }

    void SetNull() { vchLinkOffer.clear(); vchAcceptRand.clear(); nHeight = nPrice = nQty = 0; txHash.SetNull(); txBTCId.SetNull(); vchBuyerKey.clear(); bRefunded=false;vchLinkOfferAccept.clear();}
    bool IsNull() const { return (vchLinkOffer.empty() && vchAcceptRand.empty() && txHash.IsNull() && nHeight == 0 && nPrice == 0 && nQty == 0 && bRefunded == false  && txBTCId.IsNull() && vchBuyerKey.empty()); }

};
class COfferLinkWhitelistEntry {
public:
	std::vector<unsigned char> certLinkVchRand;
	char nDiscountPct;
	COfferLinkWhitelistEntry() {
		SetNull();
	}

	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(certLinkVchRand);
		READWRITE(VARINT(nDiscountPct));
	}

    friend bool operator==(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
        return (
           a.certLinkVchRand == b.certLinkVchRand
		&& a.nDiscountPct == b.nDiscountPct
        );
    }

    COfferLinkWhitelistEntry operator=(const COfferLinkWhitelistEntry &b) {
    	certLinkVchRand = b.certLinkVchRand;
		nDiscountPct = b.nDiscountPct;
        return *this;
    }

    friend bool operator!=(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
        return !(a == b);
    }
    
    void SetNull() { certLinkVchRand.clear(); nDiscountPct = 0;}
    bool IsNull() const { return (certLinkVchRand.empty() && nDiscountPct == 0); }

};
class COfferLinkWhitelist {
public:
	std::vector<COfferLinkWhitelistEntry> entries;
	bool bExclusiveResell;
	COfferLinkWhitelist() {
		SetNull();
	}

	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(entries);
		READWRITE(bExclusiveResell);

	}
    bool GetLinkEntryByHash(const std::vector<unsigned char> &ahash, COfferLinkWhitelistEntry &entry) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		if(entries[i].certLinkVchRand == ahash) {
    			entry = entries[i];
    			return true;
    		}
    	}
    	return false;
    }
    bool RemoveWhitelistEntry(const std::vector<unsigned char> &ahash) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		if(entries[i].certLinkVchRand == ahash) {
    			return entries.erase(entries.begin()+i) != entries.end();
    		}
    	}
    	return false;
    }	
    void PutWhitelistEntry(const COfferLinkWhitelistEntry &theEntry) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		COfferLinkWhitelistEntry entry = entries[i];
    		if(theEntry.certLinkVchRand == entry.certLinkVchRand) {
    			entries[i] = theEntry;
    			return;
    		}
    	}
    	entries.push_back(theEntry);
    }
    friend bool operator==(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
        return (
           a.entries == b.entries
		&& a.bExclusiveResell == b.bExclusiveResell

        );
    }

    COfferLinkWhitelist operator=(const COfferLinkWhitelist &b) {
    	entries = b.entries;
		bExclusiveResell = b.bExclusiveResell;
        return *this;
    }

    friend bool operator!=(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
        return !(a == b);
    }
    
    void SetNull() { entries.clear();}
    bool IsNull() const { return (entries.empty());}

};
class COffer {

public:
	std::vector<unsigned char> vchPubKey;
    uint256 txHash;
    uint64_t nHeight;
	std::vector<unsigned char> sCategory;
	std::vector<unsigned char> sTitle;
	std::vector<unsigned char> sDescription;
	float nPrice;
	unsigned char nCommission;
	unsigned int nQty;
	COfferAccept accept;
	std::vector<unsigned char> vchLinkOffer;
	std::vector<unsigned char> sCurrencyCode;
	std::vector<unsigned char> vchCert;
	COfferLinkWhitelist linkWhitelist;
	std::vector<std::vector<unsigned char> > offerLinks;
	bool bPrivate;
	bool bOnlyAcceptBTC;
	COffer() { 
        SetNull();
    }

    COffer(const CTransaction &tx) {
        SetNull();
        UnserializeFromTx(tx);
    }
	// clear everything but the necessary information for an offer to prepare it to go into a txn
	void ClearOffer()
	{
		accept.SetNull();
		linkWhitelist.SetNull();
		offerLinks.clear();
		sCategory.clear();
		sTitle.clear();
		sDescription.clear();
	}

 	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(sCategory);
        READWRITE(sTitle);
        READWRITE(sDescription);
		READWRITE(txHash);
		READWRITE(VARINT(nHeight));
    	READWRITE(nPrice);
    	READWRITE(VARINT(nQty));
    	READWRITE(accept);
		READWRITE(vchLinkOffer);
		READWRITE(linkWhitelist);
		READWRITE(sCurrencyCode);
		READWRITE(VARINT(nCommission));
		READWRITE(offerLinks);
		READWRITE(vchPubKey);
		READWRITE(vchCert);
		READWRITE(bPrivate);
		READWRITE(bOnlyAcceptBTC);
		
		
	}
	float GetPrice(const COfferLinkWhitelistEntry& entry=COfferLinkWhitelistEntry()){
		float price = nPrice;
		if(price==0 && !accept.IsNull())
			return accept.nPrice;
		float fCommission = nCommission;
		if(!entry.IsNull())
		{
			float fDiscount = entry.nDiscountPct;
			if(entry.nDiscountPct < -99 || entry.nDiscountPct > 99)
				fDiscount = 0;
			fDiscount = price*(fDiscount / 100);
			price = price - fDiscount;

		}
		// add commission
		fCommission = price*(fCommission / 100);
		price = price + fCommission;
		return price;
	}
	void SetPrice(float price){
		nPrice = price;
	}
    void PutToOfferList(std::vector<COffer> &offerList) {
        for(unsigned int i=0;i<offerList.size();i++) {
            COffer o = offerList[i];
            if(o.nHeight == nHeight) {
                offerList[i] = *this;
                return;
            }
        }
        offerList.push_back(*this);
    }

    bool GetOfferFromList(const std::vector<COffer> &offerList) {
        if(offerList.size() == 0) return false;
		COffer myOffer = offerList.front();
		// find the closes offer without going over in height, assuming offerList orders entries by nHeight ascending
        for(unsigned int i=0;i<offerList.size();i++) {
            COffer o = offerList[i];
            if(o.nHeight < nHeight) {
                myOffer = offerList[i];
            }
			else if(o.nHeight >= nHeight)
			{
				myOffer = offerList[i];
				break;
			}
        }
        *this = myOffer;
        return true;
    }

    friend bool operator==(const COffer &a, const COffer &b) {
        return (
         a.sCategory==b.sCategory
        && a.sTitle == b.sTitle 
        && a.sDescription == b.sDescription 
        && a.nPrice == b.nPrice 
        && a.nQty == b.nQty 
        && a.txHash == b.txHash
        && a.nHeight == b.nHeight
        && a.accept == b.accept
		&& a.vchLinkOffer == b.vchLinkOffer
		&& a.linkWhitelist == b.linkWhitelist
		&& a.sCurrencyCode == b.sCurrencyCode
		&& a.nCommission == b.nCommission
		&& a.vchPubKey == b.vchPubKey
		&& a.vchCert == b.vchCert
		&& a.bPrivate == b.bPrivate
		&& a.bOnlyAcceptBTC == b.bOnlyAcceptBTC
		
        );
    }

    COffer operator=(const COffer &b) {
        sCategory = b.sCategory;
        sTitle = b.sTitle;
        sDescription = b.sDescription;
        nPrice = b.nPrice;
        nQty = b.nQty;
        txHash = b.txHash;
        nHeight = b.nHeight;
        accept = b.accept;
		vchLinkOffer = b.vchLinkOffer;
		linkWhitelist = b.linkWhitelist;
		sCurrencyCode = b.sCurrencyCode;
		offerLinks = b.offerLinks;
		nCommission = b.nCommission;
		vchPubKey = b.vchPubKey;
		vchCert = b.vchCert;
		bPrivate = b.bPrivate;
		bOnlyAcceptBTC = b.bOnlyAcceptBTC;
        return *this;
    }

    friend bool operator!=(const COffer &a, const COffer &b) {
        return !(a == b);
    }
    
    void SetNull() { nHeight = nPrice = nQty = 0; txHash.SetNull(); bPrivate = false; bOnlyAcceptBTC = false; accept.SetNull(); sTitle.clear(); sDescription.clear();vchLinkOffer.clear();linkWhitelist.SetNull();sCurrencyCode.clear();offerLinks.clear();nCommission=0;vchPubKey.clear();vchCert.clear();}
    bool IsNull() const { return (vchPubKey.empty() && txHash.IsNull() && nHeight == 0 && nPrice == 0 && nQty == 0 &&  linkWhitelist.IsNull() && offerLinks.empty() && nCommission == 0 && bPrivate == false && bOnlyAcceptBTC == false); }

    bool UnserializeFromTx(const CTransaction &tx);
	const std::vector<unsigned char> Serialize();
};

class COfferDB : public CDBWrapper {
public:
	COfferDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "offers", nCacheSize, fMemory, fWipe) {}

	bool WriteOffer(const std::vector<unsigned char>& name, std::vector<COffer>& vtxPos) {
		return Write(make_pair(std::string("offeri"), name), vtxPos);
	}

	bool EraseOffer(const std::vector<unsigned char>& name) {
	    return Erase(make_pair(std::string("offeri"), name));
	}

	bool ReadOffer(const std::vector<unsigned char>& name, std::vector<COffer>& vtxPos) {
		return Read(make_pair(std::string("offeri"), name), vtxPos);
	}

	bool ExistsOffer(const std::vector<unsigned char>& name) {
	    return Exists(make_pair(std::string("offeri"), name));
	}


    bool ScanOffers(
            const std::vector<unsigned char>& vchName,
            unsigned int nMax,
            std::vector<std::pair<std::vector<unsigned char>, COffer> >& offerScan);

    bool ReconstructOfferIndex(CBlockIndex *pindexRescan);
};
void PutOfferAccept(std::vector<COffer> &offerList, COffer& theOffer, const COfferAccept &theOA);
bool GetAcceptByHash(const std::vector<COffer> &offerList,  COfferAccept &ca);
bool GetTxOfOfferAccept(COfferDB& dbOffer, const std::vector<unsigned char> &vchOffer, const std::vector<unsigned char> &vchOfferAccept,
		COfferAccept &theOfferAccept, CTransaction& tx);
bool GetTxOfOffer(COfferDB& dbOffer, const std::vector<unsigned char> &vchOffer, COffer& txPos, CTransaction& tx);
#endif // OFFER_H
