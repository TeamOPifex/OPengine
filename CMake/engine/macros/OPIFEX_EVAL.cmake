
macro(OPIFEX_EVAL name)
   IF(${ARGN})
     SET(${name} 1)
   ELSE(${ARGN})
     SET(${name} 0)
   ENDIF(${ARGN})
endmacro(OPIFEX_EVAL)
