#ifndef _CS309_A4_POLAR_H_
#define _CS309_A4_POLAR_H_

/**
 * @brief a reprsentation of polar coordiates
 * @member theta_deg angle in degrees
 * @member r radius between 0 and 1
 */
typedef struct {
    float theta_deg; 	/* [0..360] */
    float r;  			/* [0..1] */
} polar_t;

/** 
 * @brief comparison operator for polar_t's
 * @return true if the polar_t's are equal
 */ 
bool operator==(const polar_t& lhs, const polar_t& rhs)
{
	return rhs.theta_deg == lhs.theta_deg && rhs.r == lhs.r; 
}

/** 
 * @breif a value indicating that the value is not a real polar coordinate. 
 */
static const polar_t POLAR_NIL = {-1.0, -1.0}; 

#endif // _CS309_A4_POLAR_H_
