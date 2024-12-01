import unittest

import subprocess

def process_orders(order_sequence):
    """
    Calls the external program "engine" and passes orders via stdin.

    Args:
        order_sequence (list): A list of order strings, each formatted as:
            <Trader Identifier> <Side> <Quantity> <Price>

    Returns:
        list: A list of resulting trades, one line per aggressor execution.
    """
    try:
        # Run the "engine" program
        process = subprocess.Popen(
            ["./run"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Join the input orders with newline and pass to stdin
        input_data = "\n".join(order_sequence)
        stdout, stderr = process.communicate(input=input_data)

        # Check for errors
        if process.returncode != 0:
            raise RuntimeError(f"Engine error: {stderr.strip()}")

        # Split the output into lines
        return [line.strip() for line in stdout.strip().split("\n")[1:] if line.strip()]

    except FileNotFoundError:
        raise RuntimeError("The program 'engine' was not found. Ensure it is installed and in the PATH.")
    except Exception as e:
        raise RuntimeError(f"An error occurred while running the engine: {e}")


class TestOrderMatchingEngine(unittest.TestCase):
    def test_single_match(self):
        orders = [
            "T1 B 5 50",
            "T2 S 5 50"
        ]
        expected = ["T1+5@50 T2-5@50"]
        self.assertEqual(process_orders(orders), expected)

    def test_no_match_resting(self):
        orders1 = [
            "T1 B 5 40",
            "T2 S 5 50"
        ]
        expected = []

        orders2=[
            "T1 B 5 30",
            "T2 S 5 50"
        ]
        self.assertEqual(process_orders(orders1), expected)
        self.assertEqual(process_orders(orders2), expected)

    def test_partial_match(self):
        orders = [
            "T1 B 5 60",
            "T2 S 3 60",
            "T3 S 4 60"
        ]
        expected = ["T1+3@60 T2-3@60", "T1+2@60 T3-2@60"]
        self.assertEqual(process_orders(orders), expected)

    def test_multiple_trades_one_line(self):
        orders = [
            "T1 B 6 70",
            "T2 S 3 70",
            "T3 S 3 70",
            "T4 S 1 70"
        ]
        expected = ["T1+3@70 T2-3@70", "T1+3@70 T3-3@70"]
        self.assertEqual(process_orders(orders), expected)

    def test_trade_aggregation(self):
        orders = [
            "T1 B 6 70",
            "T2 S 3 70",
            "T3 S 3 70",
            "T4 S 1 70",
            "T1 B 4 80",
            "T3 S 2 80"
        ]

        expected = [
            "T1+3@70 T2-3@70",
            "T1+3@70 T3-3@70",
            "T1+1@70 T4-1@70",
            "T1+2@80 T3-2@80"
        ]
        self.assertEqual(process_orders(orders), expected)

    def test_complex_case(self):
        orders= [
            "T1 B 5 30",
            "T2 S 5 70",
            "T3 B 1 40",
            "T4 S 2 60",
            "T5 S 3 70",
            "T6 S 20 80",
            "T7 S 1 50",
            "T2 S 5 70",
            "T1 B 1 50",
            "T1 B 3 60",
            "T7 S 2 50",
            "T8 B 10 90"
        ]
        expected = [
            "T1+1@50 T7-1@50",
            "T1+2@60 T4-2@60",
            "T1+1@60 T7-1@60",
            "T2-6@70 T5-3@70 T7-1@50 T8+1@50 T8+9@70"
        ]
        self.assertEqual(process_orders(orders), expected)

    def test_multiple_matching_with_best_price(self):
        orders = [
            "T1 B 6 50",
            "T2 S 3 50",
            "T3 S 2 50"
        ]
        expected = [
            "T1+3@50 T2-3@50",
            "T1+2@50 T3-2@50"
        ]
        self.assertEqual(process_orders(orders), expected)

    def test_aggressor_stop(self):
        orders = [
            "T1 B 8 50",
            "T2 S 3 40",
            "T3 S 4 60",
            "T4 S 2 50"
        ]
        expected = [
            "T1+3@50 T2-3@50",
            "T1+2@50 T4-2@50"
        ]
        self.assertEqual(process_orders(orders), expected)



    def test_match_against_multiple_level(self):
        orders = [
            "T1 B 10 60",
            "T2 S 3 50",
            "T3 S 2 60",
            "T4 S 5 70"
        ]

        expected = [
            "T1+3@60 T2-3@60",
            "T1+2@60 T3-2@60"
        ]
        self.assertEqual(process_orders(orders), expected)

    def test_my(self):
        orders = [
            "T1 S 3 50",
            "T2 S 5 60",
            "T3 B 2 40",
            "T4 S 2 3",
            "T1 B 2 80",
            "T5 B 20 90"
        ]

        expected = [
            "T3+2@40 T4-2@40",
            "T1+2@60 T2-2@60",
            "T1-3@50 T2-3@60 T5+3@50 T5+3@60"
        ]
        self.assertEqual(process_orders(orders), expected)


if __name__ == "__main__":
    unittest.main()
