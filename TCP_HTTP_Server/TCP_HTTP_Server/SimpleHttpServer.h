#include "HttpStack.h"

class CSimpleHttpServer : public IHttpStackCallBack
{
public:
	CSimpleHttpServer();
	virtual ~CSimpleHttpServer();

	virtual bool RecvHttpRequest(CHttpMessage* pclsRequest, CHttpMessage* pclsResponse);

	std::string m_strDocumentRoot;
	bool m_bStop;
};