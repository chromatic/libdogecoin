/**********************************************************************
 * Copyright (c) 2022 bluezr                                          *
 * Copyright (c) 2022 The Dogecoin Foundation                         *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <test/utest.h>

#include <dogecoin/address.h>
#include <dogecoin/buffer.h>
#include <dogecoin/transaction.h>
#include <dogecoin/tx.h>
#include <dogecoin/utils.h>

void test_transaction()
{
    char* private_key_wif = "ci5prbqz7jXyFPVWKkHhPq4a9N8Dag3TpeRfuqqC2Nfr7gSqx1fy";
    char* public_key_hex = "031dc1e49cfa6ae15edd6fa871a91b1f768e6f6cab06bf7a87ac0d8beb9229075b";
    char* internal_p2pkh_address = "noxKJyGPugPRN4wqvrwsrtYXuQCk7yQEsy";

    char* external_p2pkh_address = "nbGfXLskPh7eM1iG5zz5EfDkkNTo9TRmde";
    char* utxo_scriptpubkey = "76a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac";

    char* utxo_txid_from_tx_worth_2_dogecoin = "b4455e7b7b7acb51fb6feba7a2702c42a5100f61f61abafa31851ed6ae076074";
    char* utxo_txid_from_tx_worth_10_dogecoin = "42113bdc65fc2943cf0359ea1a24ced0b6b0b5290db4c63a3329c6601c4616e2";

    int utxo_previous_output_index_from_tx_worth_2_dogecoin = 1;
    int utxo_previous_output_index_from_tx_worth_10_dogecoin = 1;

    const char* raw_hexadecimal_transaction_from_tx_worth_2_dogecoin = "0100000001e298a076ea26489c4ea60b34cb79a386a16aeef17cd646e9bdc3e4486b4abadf0100000068453042021e623cf9ebc2e2736343827c2dda22a85c41347d5fe17e4a1dfa57ebb3eb0e022075baa343944021a24a8a99c5a90b3af2fd47b92bd1e1fe0f7dc1a5cb95086df0012102ac1447c59fd7b96cee31e4a22ec051cf393d76bc3f275bcd5aa7580377d32e14feffffff02208d360b890000001976a914a4a942c99c94522a025b2b8cfd2edd149fb4995488ac00c2eb0b000000001976a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac96fe3700";
    const char* raw_hexadecimal_transaction_from_tx_worth_10_dogecoin = "01000000011b557be8ca232244085641b91d6a587ebaf227d7dd1db4c578b3a3878ac2c676010000006a4730440220739ee157e98f60eda768fb473168fb6b25878572e9aaa9d2593ef1217291558e02206d0da7f862571f6826d5cacea408445b934c1191cde77c46e146ad8b867250d70121024b67a792594a459d525d50dd4d4fb21a792c0241596d522ed627cabf0ed3d4abfeffffff02600c39fab91400001976a9141476c35e582eb198e1a28c455005a70c6869586888ac00ca9a3b000000001976a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac95fe3700";

    const char* our_unsigned_hexadecimal_transaction = "0100000002746007aed61e8531faba1af6610f10a5422c70a2a7eb6ffb51cb7a7b7b5e45b40100000000ffffffffe216461c60c629333ac6b40d29b5b0b6d0ce241aea5903cf4329fc65dc3b11420100000000ffffffff020065cd1d000000001976a9144da2f8202789567d402f7f717c01d98837e4325488ac30b4b529000000001976a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac00000000";
    const char* our_expected_single_input_signed_transaction = "0100000002746007aed61e8531faba1af6610f10a5422c70a2a7eb6ffb51cb7a7b7b5e45b4010000006b48304502210090bddac300243d16dca5e38ab6c80d5848e0d710d77702223bacd6682654f6fe02201b5c2e8b1143d8a807d604dc18068b4278facce561c302b0c66a4f2a5a4aa66f0121031dc1e49cfa6ae15edd6fa871a91b1f768e6f6cab06bf7a87ac0d8beb9229075bffffffffe216461c60c629333ac6b40d29b5b0b6d0ce241aea5903cf4329fc65dc3b11420100000000ffffffff020065cd1d000000001976a9144da2f8202789567d402f7f717c01d98837e4325488ac30b4b529000000001976a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac00000000";
    const char* our_expected_signed_raw_hexadecimal_transaction = "0100000002746007aed61e8531faba1af6610f10a5422c70a2a7eb6ffb51cb7a7b7b5e45b4010000006b48304502210090bddac300243d16dca5e38ab6c80d5848e0d710d77702223bacd6682654f6fe02201b5c2e8b1143d8a807d604dc18068b4278facce561c302b0c66a4f2a5a4aa66f0121031dc1e49cfa6ae15edd6fa871a91b1f768e6f6cab06bf7a87ac0d8beb9229075bffffffffe216461c60c629333ac6b40d29b5b0b6d0ce241aea5903cf4329fc65dc3b1142010000006a47304402200e19c2a66846109aaae4d29376040fc4f7af1a519156fe8da543dc6f03bb50a102203a27495aba9eead2f154e44c25b52ccbbedef084f0caf1deedaca87efd77e4e70121031dc1e49cfa6ae15edd6fa871a91b1f768e6f6cab06bf7a87ac0d8beb9229075bffffffff020065cd1d000000001976a9144da2f8202789567d402f7f717c01d98837e4325488ac30b4b529000000001976a914d8c43e6f68ca4ea1e9b93da2d1e3a95118fa4a7c88ac00000000";

// -------------------------------- address validation --------------------------------
    // prove internal p2pkh was derived from private key in wif format which
    // validates libdogecoins addressing functionality as both were generated by dogecoin core:
    u_assert_int_eq(verifyPrivPubKeypair(private_key_wif, internal_p2pkh_address, true), 1);

    // prove internal p2pkh address was derived from public key hex:
    size_t sizeout = 33; // public hexadecimal keys are 66 characters long (divided by 2 for byte size)
    char p2pkh_pubkey_internal[sizeout];
    dogecoin_pubkey pubkeytx;
    dogecoin_pubkey_init(&pubkeytx);
    pubkeytx.compressed = true;

    // convert our public key hex to byte array:
    uint8_t* pubkeydat = utils_hex_to_uint8(public_key_hex);

    // copy byte array pubkeydat to dogecoin_pubkey.pubkey:
    memcpy(&pubkeytx.pubkey, pubkeydat, sizeout);
    
    // derive p2pkh address from new injected dogecoin_pubkey with known hexadecimal public key:
    dogecoin_pubkey_getaddr_p2pkh(&pubkeytx, &dogecoin_chainparams_test, p2pkh_pubkey_internal);
    printf("p2pkh_pubkey: %s\n", p2pkh_pubkey_internal);

    // assert the p2pkh address we just generated matches the one from dogecoin core:
    u_assert_str_eq(internal_p2pkh_address, p2pkh_pubkey_internal);

    // validate p2pkh we will send 5 dogecoin to:
    u_assert_int_eq(verifyP2pkhAddress(external_p2pkh_address, strlen(external_p2pkh_address)), 1);

// -------------------------------- transaction generation & validation --------------------------------

    // instantiate a new working_transaction object by calling start_transaction()
    // which passes back index and stores in index variable
    int working_transaction_index = start_transaction();
    // add 1st input worth 2 dogecoin:
    add_utxo(1, utxo_txid_from_tx_worth_2_dogecoin, utxo_previous_output_index_from_tx_worth_2_dogecoin);

    // get raw hexadecimal transaction to sign in the next steps
    char* raw_hexadecimal_transaction = get_raw_transaction(1);

    printf("raw transaction: %s\n", raw_hexadecimal_transaction);
    printf("raw_hexadecimal_transaction_from_tx_worth_2_dogecoin: %s\n", raw_hexadecimal_transaction_from_tx_worth_2_dogecoin);

    // add 2nd input worth 10 dogecoin:
    add_utxo(1, utxo_txid_from_tx_worth_10_dogecoin, utxo_previous_output_index_from_tx_worth_10_dogecoin);

    // add output to transaction which is amount and address we are sending to:
    add_output(working_transaction_index, external_p2pkh_address, 500000000);
    // manually make change and send back to public key address
    make_change(working_transaction_index, public_key_hex, external_p2pkh_address, 226000, 700000000);

    // get updated raw hexadecimal transaction
    raw_hexadecimal_transaction = get_raw_transaction(1);

    // assert complete raw hexadecimal transaction is equal to expected our_unsigned_hexadecimal_transaction
    u_assert_str_eq(raw_hexadecimal_transaction, our_unsigned_hexadecimal_transaction);

    // confirm total output value equals total utxo input value minus transaction fee
    // validate external p2pkh address by converting script hash to p2pkh and asserting equal:
    raw_hexadecimal_transaction = finalize_transaction(working_transaction_index, external_p2pkh_address, 226000, 1200000000);
    
    // sign current working transaction input index 0 of raw tx hex with script pubkey from utxo with sighash type of 1 (SIGHASH_ALL),
    // amount of 2 dogecoin represented as koinu (multiplied by 100 million) and with private key in wif format
    u_assert_int_eq(sign_raw_transaction(0, raw_hexadecimal_transaction, utxo_scriptpubkey, 1, 200000000, private_key_wif), 1);

    // assert that our hexadecimal buffer (raw_hexadecimal_transaction) is equal to the expected transaction
    // with the first input signed:
    u_assert_str_eq(raw_hexadecimal_transaction, our_expected_single_input_signed_transaction);

    // sign current working transaction input index 1 of raw tx hex with script pubkey from utxo with sighash type of 1 (SIGHASH_ALL),
    // amount of 10 dogecoin represented as koinu (multiplied by 100 million) and with private key in wif format
    u_assert_int_eq(sign_raw_transaction(1, raw_hexadecimal_transaction, utxo_scriptpubkey, 1, 1000000000, private_key_wif), 1);

    // assert that our hexadecimal bufer (raw_hexadecimal_transaction) is equal to the expected finalized
    // transaction with both inputs signed:
    u_assert_str_eq(raw_hexadecimal_transaction, our_expected_signed_raw_hexadecimal_transaction);

    // remove working transaction object from hashmap
    clear_transaction(working_transaction_index);
}
