#include "pullingData.h"
#include "config.h"

std::vector<Domain> queryAllDomains();
std::vector<Domain> queryCategoryDomains(std::string category);
std::vector<Domain> readLocalData();
void verifyLocalData(std::vector<Domain>* localDataPointer);
void updateLocalData(std::vector<Domain>* localDataPointer);

std::vector<Domain> readLocalData() {


    std::vector<Domain> temp;

    return temp;
}

void verifyLocalData(std::vector<Domain>* localDataPointer) {
    


}

void updateLocalData(std::vector<Domain>* localDataPointer) {

}



std::vector<Domain> pullDomains(DomainQueryType domainQueryType, std::optional<std::string> categoryInput) {
    std::vector<Domain> localData = readLocalData();

    if (isOnline) {
        verifyLocalData(&localData); 
    }
    
    std::vector<Domain> output;
    
    switch (domainQueryType) {
        case DomainQueryType::ALL:
            output = queryAllDomains();

            break;

        case DomainQueryType::CATEGORY:
            output = queryCategoryDomains(categoryInput.value());

            break;
    }


    return output;
}

std::vector<Domain> queryAllDomains() {

    
    std::vector<Domain> temp;

    return temp;
}


std::vector<Domain> queryCategoryDomains(std::string category) {

    
    std::vector<Domain> temp;

    return temp;
}
