#pragma once
#define LINK_ABOVE 0
#define LINK_NEXT 1
template <typename T>
//这是循环链表
class LinkList
{
public:
	T data;
	LinkList* above;
	LinkList* next;
	LinkList()
	{
		data = T();
		above = this;
		next = this;
	}
	LinkList* FindNode(long long address)
	{
		LinkList* p = this;
		while (address > 0)
		{
			p = p->next;
			address--;
		}
		while (-address > 0)
		{
			p = p->above;
			address++;
		}
		return p;
	}
	bool AddNode(T Data, long long address = -1, char mode = LINK_NEXT)
	{
		LinkList* p = FindNode(address);
		if (mode == LINK_ABOVE)
		{
			p->above->next = new LinkList;
			if (!p->above->next)return 0;
			p->above->next->above = p->above;
			p->above->next->next = p;
			p->above = p->above->next;
			p->above->data = Data;
			return 1;
		}
		else if (mode == LINK_NEXT)
		{
			p->next->above = new LinkList;
			if (!p->next->above)return 0;
			p->next->above->next = p->next;
			p->next->above->above = p;
			p->next = p->next->above;
			p->next->data = Data;
			return 1;
		}
		return 0;
	}
	bool push_back(const T data)
	{
		return AddNode(data);
	}
	bool DeleteNode(long long address)
	{
		LinkList* p = FindNode(address + 1);
		if (p->above == this)
		{
			return 0;
		}
		p->above->above->next = p;
		p = p->above->above;
		delete p->next->above;
		p->next->above = p;
		return true;
	}
	bool FixNode(T Data, long long address)
	{
		LinkList* p = FindNode(address);
		p->data = Data;
		return 1;
	}
	void FixLink(LinkList Link, long long address = 1)
	{
		LinkList* p = this->FindNode(address);
		LinkList* pl = Link.FindNode(1);
		for (int i = 1; i <= Link.Long(); i++)
		{
			p->data = pl->data;
			p = p->next;
			pl = pl->next;
		}
	}
	void FreeLinkList()
	{
		while (1)
		{
			if (!DeleteNode(-1))return;
		}
	}
	LinkList Intercept(long long address1, long long address2)
	{
		LinkList* p = this->FindNode(address1);
		LinkList* pr = this->FindNode(address2);
		LinkList Link;
		while (1)
		{
			if (p == pr)break;
			Link.AddNode(p->data);
			p = p->next;
		}
		return Link;
	}
	void ExchangeNode(long long address1, long long address2)
	{
		LinkList* Address1 = FindNode(address1);
		LinkList* Address2 = FindNode(address2);
		T Data = Address2->data;
		Address2->data = Address1->data;
		Address1->data = Data;
	}
	void ExchangeNode(LinkList* p1, LinkList* p2)
	{
		T data = p1->data;
		p1->data = p2->data;
		p2->data = data;
	}
	unsigned long long Long()
	{
		LinkList* p = this;
		unsigned long long Long = 0;
		while (1)
		{
			if (!p)return Long;
			p = p->above;
			if (p == this)return Long;
			Long++;
		}
	}
	unsigned long long size()
	{
		return Long();
	}
	T operator[](long long address)
	{
		return FindNode(address)->data;
	}
	void operator+(T Data)
	{
		AddNode(Data);
	}
	LinkList<int> operator=(LinkList<long long>Link)
	{
		LinkList<int> intLink;
		for (int i = 1; i <= Link.Long(); i++)
		{
			intLink.AddNode(Link[i]);
		}
		return intLink;
	}
	LinkList<long long> operator=(LinkList<int>Link)
	{
		LinkList<long long> longLink;
		for (int i = 1; i <= Link.Long(); i++)
		{
			longLink.AddNode(Link[i]);
		}
		return longLink;
	}
	LinkList<int> operator=(LinkList<char>Link)
	{
		LinkList<int> intLink;
		for (int i = 1; i <= Link.Long(); i++)
		{
			intLink.AddNode(Link[i]);
		}
		return intLink;
	}
};
