	#include "BlockHandler.h"

	BlockHandler::BlockHandler(){
		init();
	}

	void BlockHandler::init(){
		runCode = false;
		for(int tt = 0; tt < blockList_MAX; tt++){
			if(tt < IfblockList_MAX)IfblockList[tt] = NULL;
			if(tt < LoopblockList_MAX)LoopblockList[tt] = NULL;
			if(tt < LogicblockList_MAX)LogicblockList[tt] = NULL;
			if(tt < AritmeticblockList_MAX)AritmeticblockList[tt] = NULL;	
			blockList[tt] = NULL;
		}
	current = NULL;
  StartBlock = NULL;

	 blockList_N 		= 0;
	 IfblockList_N 	 = 0;
	 LoopblockList_N 	= 0;
	 LogicblockList_N 	= 0;
	 AritmeticblockList_N 	= 0;
	 Mcursor = 0;
	 messageLength = 0;
	}

  void BlockHandler::clear(){
    runCode = false;
    for(int tt = 0; tt < blockList_MAX; tt++){
      delete blockList[tt];
    }
   init();
  }
	void BlockHandler::addLoop(int id,	int startBlockID,	int endBlockID,		int count){
		 Loop *l = new Loop(id,startBlockID,endBlockID,count);
      	 blockList[blockList_N] = l;
      	 blockList_N++;
      	 LoopblockList[LoopblockList_N] = l;
      	 LoopblockList_N++;
      	 Serial.println(blockList_N);
	}

	void BlockHandler::addBlock(int id,	int _nextBlockID,int _actionID,int _intInput,int _intOutput){
		Block *block = new Block(id,_nextBlockID,_actionID,_intInput,_intOutput);
      	blockList[blockList_N] = block;
      	blockList_N++;
      	Serial.println(blockList_N);
	}

	void BlockHandler::addIf(int id,int _next_true, int _next_false, int _logic_block){
		Conditional *condition = new Conditional(id,_next_true,_next_false, _logic_block);
      	blockList[blockList_N] = condition;
      	blockList_N++;
      	IfblockList[IfblockList_N] = condition;
      	IfblockList_N++;
      	
	}

	void BlockHandler::addLogic(int id,int logicOperation,int _input_left, int _input_right){
		LogicBlock *lblock = new LogicBlock(id,logicOperation,_input_left,_input_right);
		blockList[blockList_N] = lblock;
      	blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
		
	}

	void BlockHandler::addLogicCompare(int id,int logicOperation,int _input_left, int _input_right){
		LogicCompare *lblock = new LogicCompare(id,logicOperation,_input_left,_input_right);
		blockList[blockList_N] = lblock;
      	blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
	
	}


	void BlockHandler::addConst(int id, float value){
		ConstBlock *cblock = new ConstBlock(id,value);
		blockList[blockList_N] = cblock;
      	blockList_N++;
      
	}

	/*void BlockHandler::addConst(int id, String value){
		ConstBlock *cblock = new ConstBlock(id,value);
		blockList[blockList_N] = cblock;
      	blockList_N++;
      	
	}
  */

	void BlockHandler::addAritmeticBlock(int id,int _operation,int _left,int _right){
		AritmeticBlock *ablock = new AritmeticBlock(id,_operation,_left,_right);
		blockList[blockList_N] = ablock;
		AritmeticblockList[AritmeticblockList_N] = ablock;
		blockList_N++;
		AritmeticblockList_N++;
	}

	void BlockHandler::MakeConections(){
		#if ENABLED(DEBUG_MODE)
      Serial.println("Making connections!");          
    #endif
  
		for(int ii = 0 ; ii <  blockList_N ; ii++){
			      for(int jj = 0 ; jj <  blockList_N ; jj++){
			          if(blockList[ii]->getNextID() == blockList[jj]->getID()){
			            blockList[ii]->set_next(blockList[jj]);
			            break;
			          }
			          
			    	}

			    	for(int kk = 0 ; kk <  blockList_N ; kk++){
			          if(blockList[ii]->getInputID() == blockList[kk]->getID()){
			          	blockList[ii]->set_input(blockList[kk]);
			            break;
			          }
			          
			    	}

			    	for(int tt = 0 ; tt <  blockList_N ; tt++){
			          if(blockList[ii]->getOutputID() == blockList[tt]->getID()){
			          	blockList[ii]->set_output_block(blockList[tt]);
			            break;
			          }
			          
			    	}

  		}

  		for(int kk = 0 ; kk < LogicblockList_N ; kk++){
  			LogicblockList[kk]->set_logics(blockList,blockList_N);
  		}

  		for(int kk = 0 ; kk < IfblockList_N ; kk++){
  			IfblockList[kk]->set_logics(blockList,blockList_N,LogicblockList,LogicblockList_N);
  		}

  		for(int jj = 0 ; jj < LoopblockList_N ; jj++){
  			LoopblockList[jj]->set_connections(blockList,blockList_N);
  		}

  		for(int ll = 0; ll < AritmeticblockList_N ; ll++){
  			AritmeticblockList[ll]->set_connections(blockList,blockList_N);
  		}
  		 for(int jj = 0 ; jj <  blockList_N ; jj++){
          if(1 == blockList[jj]->getID()){
            StartBlock = blockList[jj];
            break;
          }
		}
		  current = StartBlock;
	}
	bool BlockHandler::doBlock(bool loopmode){
    #ifdef DEBUG_MODE
		   		Serial.println(current->getID());
    #endif
    Block::robot->wait_And_Check_BLE_Connection(5,2);      
          current->do_action();

    #ifdef DEBUG_MODE
			 Serial.println(current->getNextID());
    #endif
		current = current->get_next(); 
			  
        if (current == NULL){
          if(loopmode){
            current = StartBlock;
            return(true);
          }else{
            return(false);
          }
          
        }

        return(true);
  }

int BlockHandler::freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

int BlockHandler::readInt(){
  int nDigits = 0;
  int sign = 1;
  while((AllMessage[Mcursor + nDigits] != ' ') && (AllMessage[Mcursor + nDigits] != '\n')){
      if(AllMessage[Mcursor + nDigits] == '-')sign = -1;
      nDigits++;
  }
  if(sign==-1){
    Mcursor++;
    nDigits--;
  }
  int out = 0;
  int power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int add = cti(AllMessage[Mcursor + ii])*power;
      out += add;
    }
    Serial.println(out);
    Mcursor += nDigits+1;
    return(out*sign);
}

int BlockHandler::cti(char x){
  int y = x - '0';
  return(y);
}

int BlockHandler::Handle_Msg(){
  int id;
  if(AllMessage[Mcursor] == 'R'){
    MakeConections();
    if(AllMessage[Mcursor+4] == 'C'){
    	return(0);
    }else{
    	return(1);
    }
    
  }
          id = readInt();
          byte startBlockID;
          byte endBlockID; 
          int countID; 
          int value;
          byte actionID;
          byte input,output;
          byte next;
          byte nextTrue,nextFalse,logicBlock;
          byte input_left,input_right, compareOperation;
  switch(AllMessage[Mcursor]){
    case 'L':
          Mcursor += 2;
          countID = readInt();
          startBlockID  = readInt();
          endBlockID = readInt();
            Serial.print("LOOP");
            Serial.print("id:");Serial.print(id);Serial.print("start:");Serial.print(startBlockID);Serial.print("end:");Serial.print(endBlockID);Serial.print(" ");Serial.println(countID);
            addLoop(id,startBlockID,endBlockID,countID);
          break;
     case 'C':
          Serial.print("CONST");
          Mcursor += 2;
          if(AllMessage[Mcursor] == 'I'){
            value = -1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'T'){
            value = 1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'F'){
            value = 0;
            Mcursor += 2;
          }else{
            value = readInt();
          }
            Serial.print("id:");Serial.print(id);Serial.print("value:");Serial.println(value);
            addConst(id,value);
          break;
      default:
      
          break;
      case 'I':
          Mcursor += 2;
          logicBlock = readInt();
          nextTrue = readInt();
          nextFalse = readInt();
          
          Serial.print("IF:");
          Serial.print("id:");Serial.print(id);
          Serial.print("true:");Serial.print(nextTrue);
          Serial.print("false:");Serial.print(nextFalse);
          Serial.print("logic:");Serial.println(logicBlock);
          addIf(id,nextTrue,nextFalse,logicBlock);
      break;

      case 'O':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	Serial.print("id:");Serial.print(id);
      	Serial.print("OperationType:");Serial.print(compareOperation);
      	Serial.print("Left:");Serial.print(input_left);
      	Serial.print("Right:");Serial.println(input_right);
      	addAritmeticBlock(id,compareOperation,input_left,input_right);
      break;

      case 'K':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	Serial.print("id:");Serial.print(id);
      	Serial.print("compType:");Serial.print(compareOperation);
      	Serial.print("Left:");Serial.print(input_left);
      	Serial.print("Right:");Serial.println(input_right);
      	addLogic(id,compareOperation,input_left,input_right);
      break;

      case 'U':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	Serial.print("id:");Serial.print(id);
      	Serial.print("compType:");Serial.print(compareOperation);
      	Serial.print("Left:");Serial.print(input_left);
      	Serial.print("Right:");Serial.println(input_right);
      	addLogicCompare(id,compareOperation,input_left,input_right);
      break;
      case 'J':
          Mcursor += 2;
          next = readInt();
          Serial.print("Starting Block ID:");
          Serial.println(next);
          addBlock(id,next,69,0,0);
       break;
         
      case 'A':
          Serial.print("ACTION:");
           Mcursor +=2;
          Serial.print("ActionID: ");
          actionID = readInt();
          input = readInt();
          output = readInt();
          next  = readInt();
          addBlock(id,next,actionID,input,output);
          Serial.print("id:");Serial.print(id);
          Serial.print("actionID:");Serial.print(actionID);
          Serial.print("input:");Serial.print(input);
          Serial.print("next:");Serial.print(next);
          Serial.print("output:");Serial.print(output);
          break;
  }

  return(2);    
}

