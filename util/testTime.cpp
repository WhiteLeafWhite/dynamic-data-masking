#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <sodium.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <cstring>
#include <vector>
#include <random>
#include <string>
#include <sstream>


using namespace std;

std::string allCharacter = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

std::string StarMasking(std::string yuanData,int pos){
    std::string ret = yuanData;
    for(int i = pos;i<yuanData.size();++i){
        ret[i] = '*';
    }
    return ret;
}

std::string ReplaceMasking(std::string yuanData){
    int temp = 1;
    if(yuanData.size() == 5){
        temp = rand()%90000+10000;
    }
    else{
        temp = rand()%9000+1000;
    }
    std::string ret = std::to_string(temp);
    return ret;
}

std::string GeneralizeMasking(std::string yuanData){
    int temp = stoi(yuanData);
    int low = temp%1000;
    int high = low + 1000;
    std::string ret = std::to_string(low) + "-" + std::to_string(high);
    return ret;
}

// 定义 Argon2id 推荐参数
#define OPS_LIMIT crypto_pwhash_OPSLIMIT_MODERATE
#define MEM_LIMIT crypto_pwhash_MEMLIMIT_MODERATE
#define HASH_LEN crypto_pwhash_BYTES // 推荐使用 32 字节哈希长度

std::string SALT = "viva odden";

// 生成密码哈希
std::string hashPassword(const std::string &password) {
    char hashedPassword[crypto_pwhash_STRBYTES];

    std::string passAndSalt = password + SALT;

    if (crypto_pwhash_str(hashedPassword, passAndSalt.c_str(), passAndSalt.size(),
                          OPS_LIMIT, MEM_LIMIT) != 0) {
        throw std::runtime_error("密码哈希失败!");
    }

    return std::string(hashedPassword);
}

std::string hashMasking(const std::string& input) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(input);
    
    std::stringstream ss;
    ss << std::hex << hashValue;
    return ss.str();
}

// Base64 编码
std::string base64_encode(const unsigned char* buffer, size_t length) {
    BIO* bio = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // 不加换行符
    bio = BIO_push(b64, bio);
    BIO_write(bio, buffer, length);
    BIO_flush(bio);

    BUF_MEM* mem_ptr;
    BIO_get_mem_ptr(bio, &mem_ptr);

    std::string encoded(mem_ptr->data, mem_ptr->length);
    BIO_free_all(bio);
    return encoded;
}

// Base64 解码
std::vector<unsigned char> base64_decode(const std::string& input) {
    BIO* bio = BIO_new_mem_buf(input.data(), input.length());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);

    std::vector<unsigned char> buffer(input.length());
    int len = BIO_read(bio, buffer.data(), input.length());
    buffer.resize(len);

    BIO_free_all(bio);
    return buffer;
}


// SM4 CBC 加密
std::string sm4_encrypt(const std::string& plaintext, const unsigned char key[17], const unsigned char iv[17]) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);

    std::vector<unsigned char> ciphertext(plaintext.size() + 16);
    int len = 0, ciphertext_len = 0;

    EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                      reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return base64_encode(ciphertext.data(), ciphertext_len);
}

std::string fpe_encrypt(const std::string& plaintext, const unsigned char key[17], const unsigned char iv[17]){
    std::string www = "123";
    std::string seeded = sm4_encrypt(www, key, iv);
    std::string output;
    int real_seed = 0;
    for(int i = 0;i<seeded.size();++i){
        real_seed+=seeded[i];
    }
    srand(real_seed);
    for(int i = 0;i<plaintext.size();++i){
        output.push_back((plaintext[i]+rand())%10+'0');
    }
    return output;
}


std::string SM4_FPEMasking(std::string yuanData, int pos){
    std::string seg = yuanData.substr(0);
    unsigned char key[17] = "1234567890abcdef";
    unsigned char iv[17]  = "abcdef1234567890";
    seg = fpe_encrypt(seg, key, iv);
    yuanData = yuanData.substr(0) + seg;
    return yuanData;
}

vector<string> datas;

int N;

int main(){
    int maskType = 0;
    srand(time(0));
    cin>>N;
    for(int i = 0;i<5;++i){
        datas.clear();
        for(int j = 1;j<=N;++j){
            string temp = "";
            if(rand()%2==1){
                for(int kk = 1 ; kk <= 4; ++kk){
                    temp.push_back(rand()%9 + '1');
                }
            }
            else{
                for(int kk = 1; kk <= 5; ++kk){
                    temp.push_back(rand()%9 + '1');
                }
            }
            datas.push_back(temp);
            //cout<<temp<<"#";
        }

        // 记录开始时间
        auto start = std::chrono::high_resolution_clock::now();

        for(int j = 0;j<N;++j){
            if(i==0){
                datas[j] = StarMasking(datas[j], 1);
            }
            if(i==1){
                datas[j] = ReplaceMasking(datas[j]);
            }
            if(i==2){
                datas[j] = hashMasking(datas[j]);
            }
            if(i==3){
                datas[j] = GeneralizeMasking(datas[j]);
            }
        }


        // 记录结束时间
        auto end = std::chrono::high_resolution_clock::now();
        
        // 计算时间差（以毫秒为单位）
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "计算所需时间: " << duration.count() << " 毫秒" << std::endl;
    }

    return 0;
}