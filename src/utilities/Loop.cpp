#include "Loop.h"

Loop::Loop() {
  counter = 0;
}

Loop::~Loop() {}

Loop::Loop(byte id, byte _begin, byte _end, byte _limit) {
  blockID = id;
  limit_block_id = _limit;
  begin_block_id = _begin;
  end_block_id = _end;
  input_block = NULL;
  next = NULL;
  output_block = NULL;
  limit_block = NULL;
  end_block = NULL;
  begin_block = NULL;
  counter = 0;

}

Loop::Loop(byte _limit){
  limit = _limit;
  counter = 0;
 
}

void Loop::do_action() {
  counter++;
  //Serial.prbyteln(counter);
}

void Loop::set_connections(Block* blockList[60],int blockList_N){
for(byte ii = 0; ii < blockList_N; ii++){
     if(blockList[ii]->getID() == begin_block_id){
            begin_block = blockList[ii];
      }else if(blockList[ii]->getID() == end_block_id){
            end_block = blockList[ii];
      }else if(blockList[ii]->getID() == limit_block_id){
        set_limit_block(blockList[ii]);
      }
  
  }
}

byte Loop::get_begin_block_id(){
  return(begin_block_id);
}
byte Loop::get_limit_block_id(){
  return(limit_block_id);
}

byte Loop::get_end_block_id(){
  return(end_block_id);
}

void Loop::set_begin_block(Block *b){
  begin_block = b;
}

void Loop::set_end_block(Block *b){
  end_block = b;
}

void Loop::set_limit_block(Block *l){
  limit_block =l;
}

void Loop::set_limit(){
  limit = limit_block->get_output();
}

Block* Loop::get_next() {
  counter++;
  Serial.print("limit:");
  Serial.println(limit);
  Serial.println(counter);
  if (counter <= limit){
    return(begin_block);
  }
  else{
    counter  = 0;
    return(end_block);
  }
}
