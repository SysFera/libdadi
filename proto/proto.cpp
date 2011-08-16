//Compilation arguments: g++ -O2 -ansi -W -Wall -I../build/include -L../ -lbotan-1.10 testPaco.cpp -o testPaco
//Read certificate with openssl: openssl x509 -in file.pem -text -noout

/*
 * Author: Eugène PAMBA CAPO-CHICHI
 *
 */

#include <botan/botan.h>
#include <botan/x509_ca.h>
#include <botan/time.h>
#include <iostream>
#include <memory>

#include <fstream>
#include <string>
#include <cstdlib>
#include <memory>
#include <botan/rsa.h>
#include <botan/dsa.h>
#include <botan/x509self.h>

#include <botan/init.h>
#include <botan/auto_rng.h>

#include <botan/x509cert.h>
#include <botan/x509stor.h>
//#include <botan/x509_crl.h>

using namespace Botan;
using namespace std;


static const string hash_fn = "SHA-512";
/*
 * This funtion generates a CA certificate by default
 * or a simple one in the other case
 */
int
generateCertificate(string certificateName, bool CAflag = true) {

  //Generation of RSA key
  //std::ofstream pub((certificateName+"_pub").c_str());
  std::ofstream priv(certificateName.c_str());

  //If the file priv and pub are not created
  //if(!priv || !pub) {
  if(!priv) {
    std::cout << "Couldn't write output files \n";
    return 1;
  }

  AutoSeeded_RNG rng;
  RSA_PrivateKey key(rng, 2048);
  //cout << "key" << key. << "\n";
  //pub << X509::PEM_encode(key);
  priv << PKCS8::PEM_encode(key);

  X509_Cert_Options opts;

  opts.common_name = "Lecoco";
  opts.country = "90";
  opts.organization = "Sysfera";
  opts.email = "Capoch@Sysfera.com";

  //Means that it is an certification authority key
  if (CAflag) {
    opts.CA_key();
  }

  X509_Certificate cert = X509::create_self_signed_cert(opts, key, hash_fn, rng);
  std::ofstream cert_file(string("Cert_"+certificateName).c_str());

  if(!cert_file) {
    std::cout << "Couldn't write output files: cert_file \n";
    return 1;
  }
  cert_file << cert.PEM_encode();
  return 0;
}

/*
 * This funtion generates the request certificate
 */
int
generateReqCertificate(string certificateName) {

  AutoSeeded_RNG rng;

  RSA_PrivateKey priv_key(rng, 2048);

  std::ofstream key_file(string("Userpriv_"+certificateName).c_str());
  key_file << PKCS8::PEM_encode(priv_key);

  X509_Cert_Options opts;

  opts.common_name = "User";
  opts.country = "90";
  opts.organization = "UserLand";
  opts.email = "User@Sysfera.com";

  PKCS10_Request req = X509::create_cert_req(opts, priv_key,
                                              hash_fn, rng);

  std::ofstream req_file(certificateName.c_str());
  if(!req_file) {
    std::cout << "Couldn't write output files: req_file \n";
    return 1;
  }
  req_file << req.PEM_encode();

  return 0;
}

/*
* This funtion signs the certificates
*/

/*
reqCertificate = pkcs10 : Public Key Cryptographic Standards
Format des messages envoyés à une autorité de certification
et demandant la signature d'une paire de clés.
*/

int
CASigns(string CACertificate,
            string CAKey,
            string reqCertificate) {

  AutoSeeded_RNG rng;

  X509_Certificate ca_cert(CACertificate);
  std::auto_ptr<PKCS8_PrivateKey> privkey(
      PKCS8::load_key(CAKey, rng)
      );
  X509_CA ca(ca_cert, *privkey, hash_fn);

  // got a request
  PKCS10_Request req(reqCertificate);
  X509_Time start_time(system_time()+2*60*60);
  X509_Time end_time(system_time() + 365 * 60 * 60 * 24);
  //X509_Time end_time(system_time() + 2*60*60);

  X509_Certificate new_cert = ca.sign_request(req, rng,
                                                  start_time, end_time);
  // send the new cert back to the requestor
  std::ofstream reqCert_signed(string("Cert_"+reqCertificate).c_str());
  reqCert_signed << new_cert.PEM_encode();

  return 0;
}


int
main() {
  try {
      string CAcertificateName = "CArsapriv.pem";
      string reqCertificateName = "req.pem";
      string reqCertificateName2 = "req2.pem";
      // Initialization
      LibraryInitializer init;

      //Generation of CA certificate
      int retCA = generateCertificate(CAcertificateName);

      if (retCA) {
        std::cerr << "Problem with the CA generation files" << "\n";
        return 1;
      }
      //Generation of the user certificate request
      int retUser = generateReqCertificate(reqCertificateName);
      generateReqCertificate(reqCertificateName2);

      if (retUser) {
        std::cerr << "Problem with the User generation files" << "\n";
        return 1;
      }

      //To generate a certificate signed after a request
      CASigns(string("Cert_"+CAcertificateName),
              CAcertificateName,
              reqCertificateName);

      CASigns(string("Cert_"+CAcertificateName),
              CAcertificateName,
              reqCertificateName2);


      //To generate certificate not signed by CA
      generateCertificate("test.pem", false);

      /*To check the certificate generated*/

      //To build the CA certificate object
      X509_Certificate ca_cert(string("Cert_"+CAcertificateName));

      //To build the certificate objects to be checked
      X509_Certificate subject1_cert("Cert_req.pem");
      X509_Certificate subject2_cert("Cert_test.pem");

      //To store the CA certificate on the list of knwon CA
      X509_Store cert_store;
      cert_store.add_cert(ca_cert, /*trusted=*/true);
      cert_store.add_cert(subject1_cert);
      cert_store.add_cert(subject2_cert);

      cout << "ca_cert is CA ==> " << ca_cert.is_CA_cert() << "\n";
      cout << "subject1_cert is CA ==> " << subject1_cert.is_CA_cert() << "\n";

      //To valide the certificates generated
      //TODO: écrire une fonction qui prend en paramètre deux certificats: CA et certificats à checker
      // Et qui retourne un booléan pour dire s'il est valide ou pas!
      X509_Code code = cert_store.validate_cert(subject1_cert);
      if(code == VERIFIED) {
        cout << "req.pem_Cert signed by the CA validated, code = " << code << "\n";
      } else {
        cout << "req.pem_Cert signed by the CA did not validate, code =" << code << "\n";
      }

      code = cert_store.validate_cert(subject2_cert);
      if(code == VERIFIED) {
        cout << "test.pem_Cert not signed by the CA validated\n";
      } else {
        cout << "test.pem_Cert not signed by the CA did not validate, code = " << code << "\n";;
      }

      /*cout <<  "algo_name of req.pem_Cert: " <<  pub->algo_name()  << "\n";
      cout <<  "content of ca_cert: \n\n" << ca_cert.to_string() << "\n\n";*/

      //To display certificates contenet
      cout <<  "content of Cert_req.pem: \n\n" << subject1_cert.to_string() << "\n\n";

      //To get the public key content
      Public_Key * pub =  subject1_cert.subject_public_key();
      cout << "public key : \n" <<  X509::PEM_encode(*pub) << "\n";
      delete pub;

      AutoSeeded_RNG rng;
      //To get the private key from file
      Private_Key* pkey= PKCS8::load_key(CAcertificateName, rng);

      //To create CA object
      X509_CA ca(ca_cert, *pkey, hash_fn);
      //To create the the revoke list crl
      X509_CRL crl1 = ca.new_crl(rng);

      if(cert_store.add_crl(crl1) != VERIFIED)
      std::cout << "\nFAILED: CRL #1 did not validate" << std::endl;

      std::vector<CRL_Entry> revoked;
      revoked.push_back(CRL_Entry(subject1_cert, KEY_COMPROMISE));

      X509_CRL crl2 = ca.update_crl(crl1, revoked, rng);

      if(cert_store.add_crl(crl2) != VERIFIED)
          std::cout << "\nFAILED: CRL #2 did not validate" << std::endl;

      code = cert_store.validate_cert(subject1_cert);
      if(code == VERIFIED) {
        cout << "req.pem_Cert signed by the CA validated, code = " << code << "\n";;
      } else {
          if (code == CERT_IS_REVOKED) {
          cout << "req.pem_Cert is revoked, code = " << code << "\n";;
          }
          else {
            cout << "req.pem_Cert did not validate, code =" << code << "\n";;
          }
      }
      delete pkey;

  } catch(std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
