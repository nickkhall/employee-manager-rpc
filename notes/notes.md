// actual rpc call (end of line)
employee_t* empman_rpc_employees_get_id(char* id)
  // query db for id

	// create employee from db reponse

	// return employee

// serialize employee data into send buffer
empman_rpc_deserialize_employees_get_id




---------------------------------------------------------------------------


request comes in
reset recv buffer
receive data into recv buffer
reset send buffer
process send buffer (w/ recv buffer to format response into)
  create rpc send buffer header
	populate send buffer header with appropriate data
	check for recv buffer rpc proc id
	  employees_get_id -> 
			deserialize recv buffer for employee id
		  query db with id
			create employee from db response
			serialize employee
send serialized employee buffer (send buffer)
reset send buffer


->deserialize id from recv buffer
->query db with id
->convert db response into employee linked list
->serialize employee linked list into send buffer

>empman_rpc_handlers_employees_get_id(ser_buff_t* recv_buffer)
  >serlib_deserialize_data_string -> into id
  >empman_rpc_db_query_by_id
	>empman_rpc_employees_convert
	>empman_rpc_serialize_employee



// 1:
// Top level, first function called when request is received
ser_buff_t* empman_rpc_handlers_employees_get_id(ser_buff_t* recv_buffer);

   // Deserialize request to grab employee id
-- serlib_deserialize_data_string();
  
   // Query database with id
-- employee_t* empman_rpc_db_query_by_id(PGresult* res, const char* const* id);
  
      // create employee linked list from db response
-- -- employee_t* empman_rpc_db_convert_response(PGresult* res, const char* const* id);

         // convert db 
-- -- -- empman_rpc_utils_convert_pq_data(employee_t* employee, PGresult* res, int row);
  
	    // serialize employee
-- -- -- -- serlib_serialize_employee_t();


