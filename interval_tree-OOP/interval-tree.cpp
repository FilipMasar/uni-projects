class interval_tree {

	class vertex {

		int value;
		int lazy; 
		int begin, end;
		vertex *left, *right;

	public:

		vertex(int b, int e) : begin(b), end(e) {	
			value = 0;
			lazy = 1;

			if(end - begin > 1) {
				int mid = (begin + end)/2;
				left = new vertex(begin, mid);
				right = new vertex(mid, end);
			} else {
				left = right = NULL;
			}
		}

		void send_info() {
			if(lazy != 1) {
				left->lazy *= lazy;
				left->value *= lazy;
				right->lazy *= lazy;
				right->value *= lazy;
				lazy = 1;
			}
		}

		void change(int i, int v) {
			if(begin == i && end == i+1) {
				value = v;
				return;
			}

			send_info();

			int mid = (begin + end)/2;
			if(i < mid) left->change(i, v);
			else right->change(i, v);
			value = left->value + right->value;
		}

		int sum(int l, int r) {
			if(l >= end || r <= begin) return 0;
			if(l <= begin && r >= end) return value;

			send_info();
			
			return left->sum(l, r) + right->sum(l, r);
		}

		void change_interval(int l, int r, int v) {
			if(l >= end || r <= begin) return;
			if(l <= begin && r >= end) {
				lazy *= v;
				value *= v;
				return;
			}

			send_info();
			
			left->change_interval(l, r, v);
			right->change_interval(l, r, v);

			value = left->value + right->value;
		}

		~vertex() {
			if(left != NULL) delete left;
			if(right != NULL) delete right;
		}
	};

// INTERVAL TREE CLASS

	vertex *root;

public:

	int size;

	interval_tree(int s) {
		size = s;
		int n = 1;
		while(n < s) n *= 2;
		root = new vertex(0, n);
	}

	void change(int i, int v) {
		root->change(i, v);
	}

	int sum(int l, int r) {
		return root->sum(l, r);
	}

	void change_interval(int l, int r, int v) {
		root->change_interval(l, r, v);
	}

	~interval_tree() {
		delete root;
	}
};
