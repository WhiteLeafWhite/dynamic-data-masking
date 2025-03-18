server:
	clang dynamic_main.cpp MysqlOperator.cpp BaseDetector.cpp PhoneDetector.cpp\
	 DetectorManager.cpp BaseMasking.cpp StarMasking.cpp AddressMasking.cpp\
	 SensitiveFieldsManager.cpp EmailMasking.cpp DbField.cpp DbTable.cpp\
	 AccountManager.cpp\
	 -I ./include\
	 -o ../server -levent -lmysqlclient -lstdc++ -std=c++11 -lsqlparser -lsodium

.PHONY:clean
clean:
	rm ../server