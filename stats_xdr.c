/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "stats.h"

bool_t
xdr_input1 (XDR *xdrs, input1 *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->y.y_val, (u_int *) &objp->y.y_len, ~0,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_input2 (XDR *xdrs, input2 *objp)
{
	register int32_t *buf;

	 if (!xdr_double (xdrs, &objp->a))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->y.y_val, (u_int *) &objp->y.y_len, ~0,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_output1 (XDR *xdrs, output1 *objp)
{
	register int32_t *buf;

	int i;

	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, ( 2 ) * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_vector (xdrs, (char *)objp->min_max, 2,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			{
				register int *genp;

				for (i = 0, genp = objp->min_max;
					i < 2; ++i) {
					IXDR_PUT_LONG(buf, *genp++);
				}
			}
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, ( 2 ) * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_vector (xdrs, (char *)objp->min_max, 2,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			{
				register int *genp;

				for (i = 0, genp = objp->min_max;
					i < 2; ++i) {
					*genp++ = IXDR_GET_LONG(buf);
				}
			}
		}
	 return TRUE;
	}

	 if (!xdr_vector (xdrs, (char *)objp->min_max, 2,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_output2 (XDR *xdrs, output2 *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->pr_aY.pr_aY_val, (u_int *) &objp->pr_aY.pr_aY_len, ~0,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	return TRUE;
}
