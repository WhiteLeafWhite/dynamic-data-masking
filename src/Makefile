server:
	clang dynamic_main.cpp MysqlOperator.cpp BaseDetector.cpp PhoneDetector.cpp\
	 DetectorManager.cpp BaseMasking.cpp StarMasking.cpp AddressMasking.cpp\
	 SensitiveFieldsManager.cpp EmailMasking.cpp GeneralizationMasking.cpp\
	 DbField.cpp DbTable.cpp StructureMasking.cpp ReplaceMasking.cpp NoiseMasking.cpp\
	 AccountManager.cpp\
	 -I ./include\
	 -o ../server -levent -lmysqlclient -lstdc++ -std=c++11 -lsqlparser -lsodium -lm

.PHONY:clean
clean:
	rm ../server